#include "WebKit.h"
#include "APIObject.h"

WebPage::WebPage(QObject *parent) : QWebPage(parent) {
    settings()->setAttribute(QWebSettings::PluginsEnabled, true);
    settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
    settings()->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, false);
    settings()->setAttribute(QWebSettings::LocalStorageEnabled, true);
    settings()->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
    settings()->setAttribute(QWebSettings::FrameFlatteningEnabled, false);

    settings()->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, true);
    settings()->setAttribute(QWebSettings::LocalContentCanAccessFileUrls, true);
    settings()->setAttribute(QWebSettings::XSSAuditingEnabled, false);

    settings()->setLocalStoragePath(qApp->applicationDirPath().append(QLatin1String("/storage")));

    settings()->setDefaultTextEncoding( "UTF-8" );
#ifdef QT_DEBUG
    settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
#endif

//    mainFrame()->addToJavaScriptWindowObject( api_->objectName(), api_);

}

WebPage::~WebPage() { }


WebView::~WebView() {
    if(webInspector_ != 0){
        webInspector_->close();
        webInspector_->deleteLater();
//        webInspector_ = 0;
//        delete webInspector_;
    }
}

//APIObject *WebView::api() {
//    return webPage_->api_;
//}


WebView::WebView(QWidget* parent) : QWebView(parent) {
    webPage_ = new WebPage(this);
//       mPage->setNetworkAccessManager(new WebNetworkAccessManager(this));

    setRenderHint(QPainter::Antialiasing, true);
    setRenderHint(QPainter::TextAntialiasing, true);



#ifdef QT_DEBUG
    webInspector_ = new QWebInspector();
    webInspector_->setPage(webPage_);
//    webInspector_->setVisible(true);
//    webInspector_->setBaseSize(600,600);
    connect(this, SIGNAL(destroyed()), webInspector_, SLOT(close()));
#endif
    setPage(webPage_);
#ifndef QT_DEBUG
    setContextMenuPolicy(Qt::NoContextMenu);
#endif

}
