var mainWindow = api.ui().mainWindow();
assert(mainWindow != null);
api.changeWindow(mainWindow);
api.wait(500);

// надо посылать в mainwindow клик, а не в объект текущий
//var actionNewProject = api.ui().widget("QToolButton", "actionNewProject");
//assert(actionNewProject != null);
//assert(api.ui().isEnabledAndVisible(actionNewProject));
//api.cursor().moveTo(actionNewProject, 50);
//api.cursor().leftButtonPress(actionNewProject);
//api.cursor().leftButtonRelease(actionNewProject);
//api.wait(5000);

//var actionFullscreen = api.ui().widget("QToolButton", "actionFullscreen");
//assert(actionFullscreen != null);
//api.cursor().moveTo(actionFullscreen, 50);
//api.cursor().leftButtonPress(actionFullscreen);
//api.cursor().leftButtonRelease(actionFullscreen);
//api.wait(200);

var logicalModelDock = api.ui().widget("QDockWidget", "logicalModelDock");
assert(logicalModelDock != null);
api.cursor().moveTo(logicalModelDock, 150);
api.cursor().leftButtonPress(logicalModelDock);
api.cursor().leftButtonRelease(logicalModelDock);
api.cursor().rightButtonPress(logicalModelDock);
api.cursor().rightButtonRelease(logicalModelDock);
api.wait(500);

var graphicalModelDock = api.ui().widget("QDockWidget", "graphicalModelDock");
assert(graphicalModelDock != null);
api.cursor().moveTo(graphicalModelDock, 150);
api.cursor().leftButtonPress(graphicalModelDock);
api.cursor().leftButtonRelease(graphicalModelDock);
api.cursor().rightButtonPress(graphicalModelDock);
api.cursor().rightButtonRelease(graphicalModelDock);
api.wait(500);

var propertyDock = api.ui().widget("QDockWidget", "propertyDock");
assert(propertyDock != null);
api.cursor().moveTo(propertyDock, 150);
api.cursor().leftButtonPress(propertyDock);
api.cursor().leftButtonRelease(propertyDock);
api.cursor().rightButtonPress(propertyDock);
api.cursor().rightButtonRelease(propertyDock);
api.wait(500);

var minimapDock = api.ui().widget("QDockWidget", "minimapDock");
assert(minimapDock != null);
api.cursor().moveTo(minimapDock, 150);
api.cursor().leftButtonPress(minimapDock);
api.cursor().leftButtonRelease(minimapDock);
api.cursor().rightButtonPress(minimapDock);
api.cursor().rightButtonRelease(minimapDock);
api.wait(500);

var paletteDock = api.ui().widget("QDockWidget", "paletteDock");
assert(paletteDock != null);
api.cursor().moveTo(paletteDock, 150);
api.cursor().leftButtonPress(paletteDock);
api.cursor().leftButtonRelease(paletteDock);
api.cursor().rightButtonPress(paletteDock);
api.cursor().rightButtonRelease(paletteDock);
api.wait(500);

var errorDock = api.ui().widget("QDockWidget", "errorDock");
assert(errorDock == null);

var menu_View = api.ui().getMenu("menu_View");
assert(menu_View != null);

api.ui().activateMenu(menu_View);
api.wait(200);

var actionMenuPanels = api.ui().getActionInMenu(menu_View, "Panels");
assert(actionMenuPanels != null);
assert(api.ui().isEnabledAndVisible(actionMenuPanels));
assert(!api.ui().actionIsCheckable(actionMenuPanels));
assert(!api.ui().actionIsChecked(actionMenuPanels));
assert(api.ui().isSubMenuInMenu(menu_View, actionMenuPanels));

var subMenuPanels = api.ui().getMenuContainedByAction(actionMenuPanels);
assert(subMenuPanels != null);

var actionErrors = api.ui().getActionInMenu(subMenuPanels, "Errors");
assert(actionErrors != null);
assert(api.ui().isEnabledAndVisible(actionErrors));
assert(api.ui().actionIsCheckable(actionErrors));
assert(!api.ui().actionIsChecked(actionErrors));
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionErrors));

api.ui().activateMenuAction(menu_View, actionMenuPanels);
api.ui().activateMenuAction(subMenuPanels, actionErrors);

errorDock = api.ui().widget("QDockWidget", "errorDock");
assert(errorDock != null);
assert(api.ui().isEnabledAndVisible(errorDock));

assert(api.ui().actionIsChecked(actionErrors));

api.cursor().moveTo(errorDock, 150);
api.cursor().leftButtonPress(errorDock);
api.cursor().leftButtonRelease(errorDock);
api.cursor().rightButtonPress(errorDock);
api.cursor().rightButtonRelease(errorDock);
api.wait(2000);
api.cursor().leftButtonPress(errorDock);
api.cursor().leftButtonRelease(errorDock);
api.wait(500);

assert(!api.ui().actionIsChecked(actionErrors));
