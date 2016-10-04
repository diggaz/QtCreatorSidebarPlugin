#include "sidebarplugin.h"
#include "sidebarconstants.h"

#include <coreplugin/icore.h>
#include <coreplugin/icontext.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/navigationwidget.h>
#include <extensionsystem/pluginmanager.h>

#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>

#include <QtPlugin>

using namespace Sidebar::Internal;

SidebarPlugin::SidebarPlugin()
    : m_isSidebarOnLeftSide(true)
{
    // Create your members
}

SidebarPlugin::~SidebarPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
}

bool SidebarPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    // Register objects in the plugin manager's object pool
    // Load settings
    // Add actions to menus
    // Connect to other plugins' signals
    // In the initialize function, a plugin can be sure that the plugins it
    // depends on have initialized their members.

    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    readSettings();

    QAction *action = new QAction(tr("Change Sidebar position"), this);
    Core::Command *cmd = Core::ActionManager::registerAction(action, Constants::ACTION_ID,
                                                             Core::Context(Core::Constants::C_GLOBAL));
    cmd->setDefaultKeySequence(QKeySequence(tr("Ctrl+Shift+P")));
    connect(action, SIGNAL(triggered()), this, SLOT(triggerAction()));

    Core::ActionManager::actionContainer(Core::Constants::M_WINDOW)->addAction(cmd, Core::Constants::G_WINDOW_SIZE);

    return true;
}

void SidebarPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.

    changePosition(QLatin1String("EditMode"), m_isSidebarOnLeftSide);
    changePosition(QLatin1String("DebugMode"), m_isSidebarOnLeftSide);
}

ExtensionSystem::IPlugin::ShutdownFlag SidebarPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)

    writeSettings();

    return SynchronousShutdown;
}

void SidebarPlugin::readSettings()
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String("Sidebar"));
    m_isSidebarOnLeftSide = settings->value(QLatin1String("isSidebarOnLeftSide"), true).toBool();
    settings->endGroup();
}

void SidebarPlugin::writeSettings()
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(QLatin1String("Sidebar"));
    settings->setValue(QLatin1String("isSidebarOnLeftSide"), m_isSidebarOnLeftSide);
    settings->endGroup();
}

template <typename T>
static T *getObject(QObject *parent)
{
    if (!parent)
        return 0;
    QList<QObject *> all = parent->children();
    foreach (QObject *obj, all) {
        if (T *result = qobject_cast<T *>(obj))
            return result;
        if (T *result = getObject<T *>(obj))
            return result;
    }
    return 0;
}

void changePosition(const QLatin1String &objectName, bool isSidebarOnLeftSide)
{
    QObject *obj = ExtensionSystem::PluginManager::getObjectByName(objectName);
    Core::IContext *context = qobject_cast<Core::IContext *>(obj);
    if (!context)
        return;

    Core::NavigationWidgetPlaceHolder *nwPlaceHolder =
            getObject<Core::NavigationWidgetPlaceHolder>(context->widget());
    if (nwPlaceHolder)
    {
        QSplitter *splitter = qobject_cast<QSplitter *>(nwPlaceHolder->parent());
        if (splitter && splitter->count() > 1)
        {
            int index = isSidebarOnLeftSide ? 0 : 1;
            if (splitter->widget(index) != nwPlaceHolder)
                splitter->insertWidget(0, splitter->widget(1));
            splitter->setStretchFactor(0, isSidebarOnLeftSide ? 0 : 1);
            splitter->setStretchFactor(1, isSidebarOnLeftSide ? 1 : 0);
        }
    }
}

void SidebarPlugin::triggerAction()
{
    m_isSidebarOnLeftSide = !m_isSidebarOnLeftSide;
    changePosition(QLatin1String("EditMode"), m_isSidebarOnLeftSide);
    changePosition(QLatin1String("DebugMode"), m_isSidebarOnLeftSide);
}
