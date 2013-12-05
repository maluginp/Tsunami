#ifndef WEBKIT_H
#define WEBKIT_H

#include <QtWebKit>

//class WebKit
//{
//public:
//    WebKit();
//};
class APIObject;
class WebPage : public QWebPage{
    Q_OBJECT
public:
    explicit WebPage(QObject* parent = 0);
    ~WebPage();
private:
    APIObject* api_;
};

class WebView : public QWebView{
Q_OBJECT
public:
    WebView(QWidget *parent = 0);
    ~WebView();
private:
    QWebPage* webPage_;
#ifdef QT_DEBUG
    QWebInspector* webInspector_;
#endif
};

#endif // WEBKIT_H
