/*
 ****************************************************************************
 * filename
 *
 * Created on: 2013-11-18
 *     Author: 贺辉
 *    License: LGPL
 *    Comment:
 *
 *
 *    =============================  Usage  =============================
 *|
 *|
 *    ===================================================================
 *
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 ****************************************************************************
 */

/*
 ***************************************************************************
 * Last Modified on: 2013-11-18
 * Last Modified by: 贺辉
 ***************************************************************************
 */


#include "messageview.h"


#include <QMetaObject>

#include <QVBoxLayout>
#include <QQuickItem>
#include <QFile>

#include "../servertime/servertime.h"
#include "../settings.h"
#include "../imageviewer/imageviewer.h"

#include <QtQml>



namespace HEHUI
{



ConversationModel::ConversationModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

QString ConversationModel::recipient() const
{
    return m_recipient;
}

void ConversationModel::setRecipient(const QString &recipient)
{

}

int ConversationModel::rowCount(const QModelIndex &parent ) const
{
    return m_messages.count();
}

int ConversationModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

QVariant ConversationModel::data(const QModelIndex &index, int role) const
{
    if(index.row() >= m_messages.size()){
     return QVariant();
    }

    Message msg = m_messages.at(index.row());
    switch (role) {
    case Qt::UserRole:
        return msg.author;

    case Qt::UserRole + 1:
        return msg.recipient;

    case Qt::UserRole + 2:
        return msg.timestamp;

    case Qt::UserRole + 3:
        return msg.message;

    default:
        break;
    }

    return QVariant();

}

QHash<int, QByteArray> ConversationModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[Qt::UserRole] = "author";
    names[Qt::UserRole + 1] = "recipient";
    names[Qt::UserRole + 2] = "timestamp";
    names[Qt::UserRole + 3] = "message";
    return names;
}

void ConversationModel::appendMessage(const QString &author, const QString &recipient, uint time, const QString &message)
{
    Message msg(author, recipient, time, message);
    m_messages.append(msg);
}






////////////////////////////////////////////////////////////////////////////////////////////




const QString URLScheme_Contact = "contact"; //contact://contact_id
const QString URLScheme_Image = "image"; //image://image_name

const QString ImagePath_Normal = "qrc:/chatmessagewindow/image.png";
const QString ImagePath_Downloading = "qrc:/chatmessagewindow/imagedownloading.gif";
const QString ImagePath_DownloadingFailed = "qrc:/chatmessagewindow/imagedownloadingfailed.png";


MessageView::MessageView(bool isGroupChat, QWidget *parent)
    : QWidget(parent), m_quickView(new QQuickView)
{


    qmlRegisterType<ConversationModel>("qml.hehui.ConversationModel", 1, 0, "ConversationModel");


//    m_conversationModel = new ConversationModel(this);
//    m_conversationModel->appendMessage("userID", myUserID, 0, "TEST Message");
//    m_quickView->rootContext()->setContextProperty("model", m_conversationModel);


    m_quickView->setResizeMode(QQuickView::SizeRootObjectToView);
    connect(m_quickView, &QQuickView::statusChanged, this, &MessageView::quickViewStatusChanged);
    connect(m_quickView, &QQuickWindow::sceneGraphError, this, &MessageView::sceneGraphError);
    QString qmlFileName = "qrc:/qml/chat.qml";
    if(isGroupChat) {
        qmlFileName = "qrc:/qml/groupchat.qml";
    }
    m_quickView->setSource(QUrl(qmlFileName));

    QWidget *container = QWidget::createWindowContainer(m_quickView);
    container->setMinimumSize(m_quickView->size());

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(container);
    setLayout(layout);


    m_myself = IMUser::instance();
    myUserID = m_myself->getUserID();


    messagesListViewer = 0;
    QQuickItem *rootItem = m_quickView->rootObject();
    messagesListViewer = rootItem->findChild<QQuickItem *>("chatMessagesList");
    if(messagesListViewer) {
        //connect(this, SIGNAL(appendMessage(const QString &, const QString &, const QString &, const QString &, uint)), messagesListViewer, SLOT(appendMessage(const QString &, const QString &, const QString &, const QString &, uint)));

        connect(messagesListViewer, SIGNAL(linkActivated(const QString &)), this, SLOT(linkClicked(const QString &)));

        messagesListViewer->setProperty("myID", myUserID);
    }


    imageCachePath = Settings::instance()->getImageCacheDir();

    m_properScrollBarValue = 0;

    lastUnACKedMessageFromContact = "";


    //connect(m_mainWebFrame, SIGNAL(contentsSizeChanged(const QSize &)), this, SLOT(scrollWebFrame(const QSize &)));


}

MessageView::~MessageView()
{
    delete m_quickView;
}

//QSize MessageView::sizeHint(){
//    return QSize(540, 300);
//}

void MessageView::appendChatMessage(const QString &userID, const QString &time, const QString &message, bool richtext)
{
    qDebug() << "----MessageView::appendChatMessage(...) contactID:" << userID << " Time:" << time << " Msg:" << message;;

//    QString timeString = datetime;
//    QDateTime dt = QDateTime::fromString(datetime, "yyyy-MM-dd hh:mm:ss");
//    if(dt.date() == ServerTime::instance()->time().date() ){
//        timeString = dt.toString("hh:mm:ss");
//    }

    //URL: contact://contactid
    //QString msg = QString("<span>%1(<a title=\"%2\" href=\"%3://%2\">%2</a>) %4</span>").arg(displayName).arg(userID).arg(URLScheme_Contact).arg(timeString);


//    emit appendMessage(userID, displayName, headICON, message, timestamp);

    if(messagesListViewer) {
        QMetaObject::invokeMethod(messagesListViewer, "appendMessage",
                                  Q_ARG(QVariant, userID),
                                  Q_ARG(QVariant, time),
                                  Q_ARG(QVariant, message)
                                 );

    }

}

void MessageView::appendHTML(const QString &htmlTag)
{
    qDebug() << "----MessageView::appendHTML(...)  htmlTag:" << htmlTag;;

}

void MessageView::setHtml(const QString &html)
{

}

void MessageView::updateImage(const QString &imageName, ImageDownloadStatus downloadStatus)
{

//    QWebElement doc = m_mainWebFrame->documentElement();
//    QWebElementCollection elements = doc.findAll("img");
//    foreach (QWebElement element, elements){

//        //QString imageSRC = element.attribute("src");
//        QString imageID = element.attribute("id").trimmed();
//        if(imageID != imageName){continue;}


//        //if(imageSRC.trimmed().startsWith("qrc:/", Qt::CaseInsensitive)){
//            switch (downloadStatus) {
//            case ImageDownloading:
//            {
//                element.setAttribute("src", ImagePath_Downloading);
//            }
//                break;
//            case ImageDownloaded:
//            {
//                element.setAttribute("src", "file://" + imageCachePath +"/"+imageName);
//                element.removeAttribute("id");
//            }
//                break;

//            case ImageDownloadingFailed:
//            {
//                element.setAttribute("src", ImagePath_DownloadingFailed);
//            }
//                break;
//            default:
//                break;
//            }

//        //}

//    }

}


void MessageView::scrollWebFrame(const QSize &contentsSize)
{


//    int frameHeight = m_mainWebFrame->geometry().size().height();
//    if(contentsSize.height() <= frameHeight){
//        m_properScrollBarValue = 0;
//        return;
//    }

//    int curScrollBarValue = m_mainWebFrame->scrollBarValue(Qt::Vertical);

//    if(m_properScrollBarValue == curScrollBarValue){
//        m_mainWebFrame->setScrollBarValue(Qt::Vertical, contentsSize.height());
//        m_properScrollBarValue = m_mainWebFrame->scrollBarValue(Qt::Vertical);
//    }else{
//        m_properScrollBarValue = contentsSize.height() - frameHeight;
//        emit signalTipLastUnACKedMessageFromContact(lastUnACKedMessageFromContact);
//    }

    lastUnACKedMessageFromContact = "";

}

void MessageView::linkClicked(const QString &urlString)
{

    QUrl url(urlString);
    QString scheme = url.scheme();

    if(scheme == URLScheme_Image) {
        QString userID = url.userInfo();
        QString imageName = url.host();


        QString localCacheImage = imageCachePath + "/" + imageName;
        if(QFile::exists(localCacheImage)) {
            //TODO:Show image
            QStringList images;
            images.append(localCacheImage);
            ImageViewer::instance()->show();
            ImageViewer::instance()->setImages(images);
        } else {
            //Download image
            updateImage(imageName, ImageDownloading);
            emit signalRequestDownloadImage(userID, imageName);
        }


    } else if(scheme == URLScheme_Contact) {
        //TODO

    }

    qDebug() << "URL scheme:" << scheme << " host:" << url.host() << " userInfo:" << url.userInfo();

}

void MessageView::quickViewStatusChanged(QQuickView::Status status)
{
//    if (status == QQuickView::Error) {
//        QStringList errors;
//        foreach (const QQmlError &error, m_quickView->errors())
//            errors.append(error.toString());
//        statusBar()->showMessage(errors.join(QStringLiteral(", ")));
//    }
}

void MessageView::sceneGraphError(QQuickWindow::SceneGraphError, const QString &message)
{
    //statusBar()->showMessage(message);
}


QString MessageView::simpleTextToRichTextMessage(const QString &simpleTextMessage)
{

    QString msg = simpleTextMessage;
    if(msg.trimmed().isEmpty()) {
        msg = "<p></p>";
        return msg;
    }

    int separateIndex = msg.indexOf(QChar('|'));
    if(separateIndex == -1) {
        return "<p></p>";
    }
    QString styleTag = msg.left(separateIndex);
    msg.remove(0, separateIndex + 1);

//    QStringList list = msg.split(QChar('|'));
//    if(list.size() != 2){return "<p></p>";}
//    QString styleTag = list.at(0);
//    msg = list.at(1);


    msg = "<p>" + msg + "</p>";
    msg.replace("\\r\\n", "</br>");

    msg.replace("\\n", "</p><p>");

    QString divStyle = simpleStyleTagToStyleString(styleTag);

    if(divStyle.trimmed().isEmpty()) {
        msg = QString("<div>") + msg;
    } else {
        msg = QString("<div style=\"%1\">").arg(divStyle) + msg;
    }
    msg += QString("</div>");

    return msg;

}

QString MessageView::contactsSimpleTextToPlainTipTextMessage(const QString &simpleTextMessage)
{


    QString msg = simpleTextMessage;
    if(msg.trimmed().isEmpty()) {
        return "";
    }

    int separateIndex = msg.indexOf(QChar('|'));
    if(separateIndex == -1) {
        return "<p></p>";
    }
    msg.remove(0, separateIndex + 1);

    msg.replace("\\r\\n", " ");
    msg.replace("\\n", " ");

    QRegExp regExp("<img.*/>");
    regExp.setCaseSensitivity(Qt::CaseInsensitive);
    regExp.setMinimal(true);
    int pos = 0;
    QStringList list;
    while ((pos = regExp.indexIn(msg, pos)) != -1) {
        list << regExp.cap(0);
        pos += regExp.matchedLength();
    }
    foreach (QString str, list) {
        msg.replace(str, tr("[Image]"));
    }

    return msg;

}

QString MessageView::simpleStyleTagToStyleString(const QString &tagsString)
{

    //tagsString FORMAT: font-family;font-size;font-weight:bold;font-style:italic;text-decoration:underline;color;

    QStringList styleList = tagsString.split(";");
    if(styleList.size() != 6) {
        return "";
    }

    QString styleInfo = "";

    QString fontFamily = styleList.at(0);
    if(!fontFamily.isEmpty()) {
        styleInfo = QString("font-family:'%1';").arg(fontFamily);
    }

    QString fontSize = styleList.at(1);
    if(!fontSize.isEmpty()) {
        styleInfo += QString("font-size:%1pt;").arg(fontSize);
    }

    QString fontBold = styleList.at(2);
    if(!fontBold.isEmpty()) {
        styleInfo += QString("font-weight:bold;");
    }

    QString fontItalic = styleList.at(3);
    if(!fontItalic.isEmpty()) {
        styleInfo += QString("font-style:italic;");
    }

    QString fontUnderline = styleList.at(4);
    if(!fontUnderline.isEmpty()) {
        styleInfo += QString("text-decoration:underline;");
    }

    QString fontColorName = styleList.at(5);
    if(!fontColorName.isEmpty()) {
        styleInfo += QString("color:%1;").arg(fontColorName);
    }

    return styleInfo;

}








} //namespace HEHUI
