#ifndef UTILS_H
#define UTILS_H

// MACROS
#define MESSAGE_WARNING(msg) QMessageBox::warning(this,windowTitle(),msg)
#define MESSAGE_INFO(msg)    QMessageBox::information(this,windowTitle(),msg)
#define MESSAGE_ERROR(msg)   QMessageBox::critical(this,windowTitle(),msg)

namespace tsunami{
namespace utils {



}
}

#endif // UTILS_H
