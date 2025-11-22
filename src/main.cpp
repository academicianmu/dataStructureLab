#include "main_window.h"
#include <QApplication>
#include <QDebug>
#include <QStyleFactory>
#include <QMessageBox>
#include <QTimer>
#include <QDir>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

int main(int argc, char *argv[])
{
    // 确保控制台输出可见
#ifdef Q_OS_WIN
    if (AttachConsole(ATTACH_PARENT_PROCESS) || AllocConsole()) {
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
    }
#endif
    
    // 设置环境变量
    qputenv("QT_LOGGING_TO_CONSOLE", "1");
    qputenv("QT_DEBUG_PLUGINS", "1");
    
    qDebug() << "=== 应用程序启动 ===";
    qDebug() << "参数:" << QCoreApplication::arguments();
    
    QApplication app(argc, argv);
    
    // 设置应用程序信息
    QApplication::setApplicationName("数据结构实验平台");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setOrganizationName("YourOrganization");
    
    // 打印调试信息
    qDebug() << "=== 系统信息 ===";
    qDebug() << "应用程序路径:" << QCoreApplication::applicationFilePath();
    qDebug() << "工作目录:" << QDir::currentPath();
    qDebug() << "库路径:" << QCoreApplication::libraryPaths();
    qDebug() << "可用样式:" << QStyleFactory::keys();
    
    // 设置Fusion样式
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    
    qDebug() << "=== 创建主窗口 ===";
    MainWindow* mainWindow = new MainWindow();
    mainWindow->setAttribute(Qt::WA_DeleteOnClose);
    
    // 使用单次定时器显示窗口，确保事件循环已启动
    QTimer::singleShot(100, [mainWindow]() {
        qDebug() << "显示主窗口...";
        mainWindow->show();
        mainWindow->activateWindow();
        mainWindow->raise();
        
        // 检查窗口状态
        qDebug() << "窗口是否可见:" << mainWindow->isVisible();
        qDebug() << "窗口是否激活:" << mainWindow->isActiveWindow();
        qDebug() << "窗口几何:" << mainWindow->geometry();
        
        // 如果窗口仍然不可见，显示错误消息
        if (!mainWindow->isVisible()) {
            QMessageBox::critical(nullptr, "错误", "无法显示主窗口！");
        }
    });
    
    qDebug() << "=== 进入事件循环 ===";
    int result = app.exec();
    qDebug() << "=== 应用程序退出，返回值:" << result;
    
    return result;
}