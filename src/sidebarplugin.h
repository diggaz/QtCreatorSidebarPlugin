#pragma once

#include "sidebar_global.h"

#include <extensionsystem/iplugin.h>

namespace Sidebar {
namespace Internal {

class SidebarPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Sidebar.json")

public:
    SidebarPlugin();
    ~SidebarPlugin();

    bool initialize(const QStringList &arguments, QString *errorString);
    void extensionsInitialized();
    ShutdownFlag aboutToShutdown();

private:
    void readSettings();
    void writeSettings();

private slots:
    void triggerAction();

private:
    bool m_isSidebarOnLeftSide;
};

} // namespace Internal
} // namespace Sidebar
