#ifndef WEBKIT_H
#define WEBKIT_H

#include <QtWebKit>
#include "defines.h"
//class WebKit
//{
//public:
//    WebKit();
//};
//namespace tsunami{
//class APIObject;
//}
class TSUNAMI_EXPORT WebPage : public QWebPage{
    friend class WebView;
    Q_OBJECT
public:
    explicit WebPage(QObject* parent = 0);
    ~WebPage();
private:

};

class TSUNAMI_EXPORT WebView : public QWebView{
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
