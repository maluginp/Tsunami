#ifndef WEBKIT_H
#define WEBKIT_H

#include <QtWebKit>

//class WebKit
//{
//public:
//    WebKit();
//};
//namespace tsunami{
//class APIObject;
//}
class WebPage : public QWebPage{
    friend class WebView;
    Q_OBJECT
public:
    explicit WebPage(QObject* parent = 0);
    ~WebPage();
private:

};

class WebView : public QWebView{
Q_OBJECT
public:
    WebView(QWidget *parent = 0);
    ~WebView();
private:
    WebPage* webPage_;
#ifdef QT_DEBUG
    QWebInspector* webInspector_;
#endif
};

#endif // WEBKIT_H
