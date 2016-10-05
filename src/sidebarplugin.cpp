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
#include <QtPlugin>

using namespace Sidebar::Internal;

namespace Sidebar {
namespace Internal {

template <typename T>
static T *getObject(QObject *parent)
{
    if (!parent)
        return 0;
    QList<QObject *> all = parent->children();
    foreach (QObject *obj, all) {
        if (T *result = qobject_cast<T *>(obj))
            return result;
        if (T *result = getObject<T>(obj))
            return result;
    }
    return 0;
}

void changePosition(const QString &objectName, bool isSidebarOnLeftSide)
{
    QObject *obj = ExtensionSystem::PluginManager::getObjectByName(objectName);
    Core::IContext *context = qobject_cast<Core::IContext *>(obj);
    if (!context)
        return;

    Core::NavigationWidgetPlaceHolder *nwPlaceHolder =
            getObject<Core::NavigationWidgetPlaceHolder>(context->widget());
    if (nwPlaceHolder){
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

}
}

SidebarPlugin::SidebarPlugin()
    : m_isSidebarOnLeftSide(true)
{
}

SidebarPlugin::~SidebarPlugin()
{
}

bool SidebarPlugin::initialize(const QStringList &/*arguments*/, QString */*errorString*/)
{
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
    changePosition(Constants::EDIT_MODE, m_isSidebarOnLeftSide);
    changePosition(Constants::DEBUG_MODE, m_isSidebarOnLeftSide);
}

ExtensionSystem::IPlugin::ShutdownFlag SidebarPlugin::aboutToShutdown()
{
    writeSettings();

    return SynchronousShutdown;
}

void SidebarPlugin::readSettings()
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(Constants::SETTINGS_GROUP);
    m_isSidebarOnLeftSide = settings->value(Constants::SETTINGS_SIDEBAR_POSITION, true).toBool();
    settings->endGroup();
}

void SidebarPlugin::writeSettings()
{
    QSettings *settings = Core::ICore::settings();
    settings->beginGroup(Constants::SETTINGS_GROUP);
    settings->setValue(Constants::SETTINGS_SIDEBAR_POSITION, m_isSidebarOnLeftSide);
    settings->endGroup();
}

void SidebarPlugin::triggerAction()
{
    m_isSidebarOnLeftSide = !m_isSidebarOnLeftSide;
    changePosition(Constants::EDIT_MODE, m_isSidebarOnLeftSide);
    changePosition(Constants::DEBUG_MODE, m_isSidebarOnLeftSide);
}
