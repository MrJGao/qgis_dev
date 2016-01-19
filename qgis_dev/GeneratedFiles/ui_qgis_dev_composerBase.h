/********************************************************************************
** Form generated from reading UI file 'qgis_dev_composerBase.ui'
**
** Created: Tue Jan 19 20:52:23 2016
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QGIS_DEV_COMPOSERBASE_H
#define UI_QGIS_DEV_COMPOSERBASE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_composerBase
{
public:
    QAction *actionNew_Project;
    QAction *actionSave_Project;
    QAction *actionZoomIn;
    QAction *actionZoomAll;
    QAction *actionZoomOut;
    QAction *actionZoomActual;
    QAction *actionPrint;
    QAction *actionExportAsImage;
    QAction *actionExportAsSvg;
    QAction *actionToggleRulers;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionShowGrid;
    QAction *actionShowFullScreen;
    QAction *actionAddArrow;
    QAction *actionPanComposer;
    QAction *actionSelectTool;
    QAction *actionAddShape;
    QAction *actionAddMap;
    QAction *actionAddHtml;
    QAction *actionExportAsPDF;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menuProject;
    QMenu *menuView;
    QMenu *menuEdit;
    QStatusBar *statusbar;
    QToolBar *m_project_toolBar;
    QToolBar *m_viewTool_toolBar;
    QToolBar *m_print_toolBar;
    QToolBar *toolBar_3;
    QToolBar *m_ItemTools_toolBar;

    void setupUi(QMainWindow *composerBase)
    {
        if (composerBase->objectName().isEmpty())
            composerBase->setObjectName(QString::fromUtf8("composerBase"));
        composerBase->resize(800, 600);
        actionNew_Project = new QAction(composerBase);
        actionNew_Project->setObjectName(QString::fromUtf8("actionNew_Project"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/themes/default/mActionNewComposer.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew_Project->setIcon(icon);
        actionSave_Project = new QAction(composerBase);
        actionSave_Project->setObjectName(QString::fromUtf8("actionSave_Project"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/themes/default/mActionFileSave.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_Project->setIcon(icon1);
        actionZoomIn = new QAction(composerBase);
        actionZoomIn->setObjectName(QString::fromUtf8("actionZoomIn"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/themes/default/mActionZoomIn.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoomIn->setIcon(icon2);
        actionZoomAll = new QAction(composerBase);
        actionZoomAll->setObjectName(QString::fromUtf8("actionZoomAll"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/themes/default/mActionZoomFullExtent.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoomAll->setIcon(icon3);
        actionZoomOut = new QAction(composerBase);
        actionZoomOut->setObjectName(QString::fromUtf8("actionZoomOut"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/themes/default/mActionZoomOut.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoomOut->setIcon(icon4);
        actionZoomActual = new QAction(composerBase);
        actionZoomActual->setObjectName(QString::fromUtf8("actionZoomActual"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/themes/default/mActionZoomActual.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionZoomActual->setIcon(icon5);
        actionPrint = new QAction(composerBase);
        actionPrint->setObjectName(QString::fromUtf8("actionPrint"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/themes/default/mActionFilePrint.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPrint->setIcon(icon6);
        actionExportAsImage = new QAction(composerBase);
        actionExportAsImage->setObjectName(QString::fromUtf8("actionExportAsImage"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/images/themes/default/mActionSaveMapAsImage.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExportAsImage->setIcon(icon7);
        actionExportAsSvg = new QAction(composerBase);
        actionExportAsSvg->setObjectName(QString::fromUtf8("actionExportAsSvg"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/images/themes/default/mActionSaveAsSVG.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExportAsSvg->setIcon(icon8);
        actionToggleRulers = new QAction(composerBase);
        actionToggleRulers->setObjectName(QString::fromUtf8("actionToggleRulers"));
        actionToggleRulers->setCheckable(true);
        actionUndo = new QAction(composerBase);
        actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/images/themes/default/mActionUndo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionUndo->setIcon(icon9);
        actionRedo = new QAction(composerBase);
        actionRedo->setObjectName(QString::fromUtf8("actionRedo"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/images/themes/default/mActionRedo.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRedo->setIcon(icon10);
        actionShowGrid = new QAction(composerBase);
        actionShowGrid->setObjectName(QString::fromUtf8("actionShowGrid"));
        actionShowGrid->setCheckable(true);
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/images/themes/default/geographic.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionShowGrid->setIcon(icon11);
        actionShowFullScreen = new QAction(composerBase);
        actionShowFullScreen->setObjectName(QString::fromUtf8("actionShowFullScreen"));
        actionShowFullScreen->setCheckable(true);
        actionAddArrow = new QAction(composerBase);
        actionAddArrow->setObjectName(QString::fromUtf8("actionAddArrow"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/images/north_arrows/gpsarrow.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionAddArrow->setIcon(icon12);
        actionPanComposer = new QAction(composerBase);
        actionPanComposer->setObjectName(QString::fromUtf8("actionPanComposer"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/images/themes/default/mActionPan.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionPanComposer->setIcon(icon13);
        actionSelectTool = new QAction(composerBase);
        actionSelectTool->setObjectName(QString::fromUtf8("actionSelectTool"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/images/themes/default/mActionSelectPolygon.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionSelectTool->setIcon(icon14);
        actionAddShape = new QAction(composerBase);
        actionAddShape->setObjectName(QString::fromUtf8("actionAddShape"));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/images/themes/default/mActionAddBasicShape.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAddShape->setIcon(icon15);
        actionAddMap = new QAction(composerBase);
        actionAddMap->setObjectName(QString::fromUtf8("actionAddMap"));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/images/themes/default/mActionAddMap.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAddMap->setIcon(icon16);
        actionAddHtml = new QAction(composerBase);
        actionAddHtml->setObjectName(QString::fromUtf8("actionAddHtml"));
        QIcon icon17;
        icon17.addFile(QString::fromUtf8(":/images/themes/default/mActionAddHtml.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAddHtml->setIcon(icon17);
        actionExportAsPDF = new QAction(composerBase);
        actionExportAsPDF->setObjectName(QString::fromUtf8("actionExportAsPDF"));
        QIcon icon18;
        icon18.addFile(QString::fromUtf8(":/images/themes/default/mActionSaveAsPDF.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExportAsPDF->setIcon(icon18);
        centralwidget = new QWidget(composerBase);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        composerBase->setCentralWidget(centralwidget);
        menubar = new QMenuBar(composerBase);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 23));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush);
        menubar->setPalette(palette);
        menubar->setMouseTracking(false);
        menubar->setAutoFillBackground(false);
        menuProject = new QMenu(menubar);
        menuProject->setObjectName(QString::fromUtf8("menuProject"));
        menuView = new QMenu(menubar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        composerBase->setMenuBar(menubar);
        statusbar = new QStatusBar(composerBase);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        composerBase->setStatusBar(statusbar);
        m_project_toolBar = new QToolBar(composerBase);
        m_project_toolBar->setObjectName(QString::fromUtf8("m_project_toolBar"));
        composerBase->addToolBar(Qt::TopToolBarArea, m_project_toolBar);
        m_viewTool_toolBar = new QToolBar(composerBase);
        m_viewTool_toolBar->setObjectName(QString::fromUtf8("m_viewTool_toolBar"));
        composerBase->addToolBar(Qt::LeftToolBarArea, m_viewTool_toolBar);
        m_print_toolBar = new QToolBar(composerBase);
        m_print_toolBar->setObjectName(QString::fromUtf8("m_print_toolBar"));
        composerBase->addToolBar(Qt::TopToolBarArea, m_print_toolBar);
        toolBar_3 = new QToolBar(composerBase);
        toolBar_3->setObjectName(QString::fromUtf8("toolBar_3"));
        composerBase->addToolBar(Qt::LeftToolBarArea, toolBar_3);
        m_ItemTools_toolBar = new QToolBar(composerBase);
        m_ItemTools_toolBar->setObjectName(QString::fromUtf8("m_ItemTools_toolBar"));
        composerBase->addToolBar(Qt::TopToolBarArea, m_ItemTools_toolBar);

        menubar->addAction(menuProject->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menubar->addAction(menuView->menuAction());
        menuProject->addAction(actionNew_Project);
        menuProject->addAction(actionSave_Project);
        menuView->addAction(actionToggleRulers);
        menuView->addAction(actionShowFullScreen);
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);
        m_project_toolBar->addAction(actionNew_Project);
        m_project_toolBar->addAction(actionSave_Project);
        m_project_toolBar->addAction(actionUndo);
        m_project_toolBar->addAction(actionRedo);
        m_viewTool_toolBar->addAction(actionPanComposer);
        m_viewTool_toolBar->addAction(actionZoomAll);
        m_viewTool_toolBar->addAction(actionZoomActual);
        m_viewTool_toolBar->addAction(actionZoomIn);
        m_viewTool_toolBar->addAction(actionZoomOut);
        m_print_toolBar->addAction(actionPrint);
        m_print_toolBar->addAction(actionExportAsImage);
        m_print_toolBar->addAction(actionExportAsSvg);
        m_print_toolBar->addAction(actionExportAsPDF);
        toolBar_3->addAction(actionShowGrid);
        m_ItemTools_toolBar->addAction(actionSelectTool);
        m_ItemTools_toolBar->addAction(actionAddArrow);
        m_ItemTools_toolBar->addAction(actionAddShape);
        m_ItemTools_toolBar->addAction(actionAddMap);
        m_ItemTools_toolBar->addAction(actionAddHtml);

        retranslateUi(composerBase);

        QMetaObject::connectSlotsByName(composerBase);
    } // setupUi

    void retranslateUi(QMainWindow *composerBase)
    {
        composerBase->setWindowTitle(QApplication::translate("composerBase", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionNew_Project->setText(QApplication::translate("composerBase", "New Project", 0, QApplication::UnicodeUTF8));
        actionSave_Project->setText(QApplication::translate("composerBase", "Save Project", 0, QApplication::UnicodeUTF8));
        actionZoomIn->setText(QApplication::translate("composerBase", "Zoom In", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionZoomIn->setToolTip(QApplication::translate("composerBase", "Zoom In", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionZoomAll->setText(QApplication::translate("composerBase", "Zoom All", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionZoomAll->setToolTip(QApplication::translate("composerBase", "Zoom Full Extent", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionZoomOut->setText(QApplication::translate("composerBase", "Zoom Out", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionZoomOut->setToolTip(QApplication::translate("composerBase", "Zoom Out", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionZoomActual->setText(QApplication::translate("composerBase", "Zoom Actual", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionZoomActual->setToolTip(QApplication::translate("composerBase", "Zoom Actual", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionPrint->setText(QApplication::translate("composerBase", "Print", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionPrint->setToolTip(QApplication::translate("composerBase", "Print Map", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionExportAsImage->setText(QApplication::translate("composerBase", "Export As Image", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionExportAsImage->setToolTip(QApplication::translate("composerBase", "Export As Image", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionExportAsSvg->setText(QApplication::translate("composerBase", "Export As Svg", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionExportAsSvg->setToolTip(QApplication::translate("composerBase", "Export As Svg", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionToggleRulers->setText(QApplication::translate("composerBase", "Show Rulers", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionToggleRulers->setToolTip(QApplication::translate("composerBase", "Show rulers", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionToggleRulers->setShortcut(QApplication::translate("composerBase", "Ctrl+R", 0, QApplication::UnicodeUTF8));
        actionUndo->setText(QApplication::translate("composerBase", "Undo", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionUndo->setToolTip(QApplication::translate("composerBase", "Revert last change", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionUndo->setShortcut(QApplication::translate("composerBase", "Ctrl+Z", 0, QApplication::UnicodeUTF8));
        actionRedo->setText(QApplication::translate("composerBase", "Redo", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionRedo->setToolTip(QApplication::translate("composerBase", "Rstore last change", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionRedo->setShortcut(QApplication::translate("composerBase", "Ctrl+Shift+Z", 0, QApplication::UnicodeUTF8));
        actionShowGrid->setText(QApplication::translate("composerBase", "Show Grid", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionShowGrid->setToolTip(QApplication::translate("composerBase", "Show grid", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionShowFullScreen->setText(QApplication::translate("composerBase", "Show Full Screen", 0, QApplication::UnicodeUTF8));
        actionShowFullScreen->setShortcut(QApplication::translate("composerBase", "F11", 0, QApplication::UnicodeUTF8));
        actionAddArrow->setText(QApplication::translate("composerBase", "Add Arrow", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionAddArrow->setToolTip(QApplication::translate("composerBase", "Add arrow", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionPanComposer->setText(QApplication::translate("composerBase", "Pan Composer", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionPanComposer->setToolTip(QApplication::translate("composerBase", "Pan composer", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionPanComposer->setShortcut(QApplication::translate("composerBase", "P", 0, QApplication::UnicodeUTF8));
        actionSelectTool->setText(QApplication::translate("composerBase", "Select Tool", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionSelectTool->setToolTip(QApplication::translate("composerBase", "Select tool", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionSelectTool->setShortcut(QApplication::translate("composerBase", "V", 0, QApplication::UnicodeUTF8));
        actionAddShape->setText(QApplication::translate("composerBase", "Add Shape", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionAddShape->setToolTip(QApplication::translate("composerBase", "Add Shape", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionAddMap->setText(QApplication::translate("composerBase", "Add Map", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionAddMap->setToolTip(QApplication::translate("composerBase", "Add map", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionAddHtml->setText(QApplication::translate("composerBase", "Add Html", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionAddHtml->setToolTip(QApplication::translate("composerBase", "Add html", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionExportAsPDF->setText(QApplication::translate("composerBase", "Export as PDF", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionExportAsPDF->setToolTip(QApplication::translate("composerBase", "Export as PDF", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        menuProject->setTitle(QApplication::translate("composerBase", "Project", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("composerBase", "View", 0, QApplication::UnicodeUTF8));
        menuEdit->setTitle(QApplication::translate("composerBase", "Edit", 0, QApplication::UnicodeUTF8));
        m_project_toolBar->setWindowTitle(QApplication::translate("composerBase", "toolBar", 0, QApplication::UnicodeUTF8));
        m_viewTool_toolBar->setWindowTitle(QApplication::translate("composerBase", "toolBar", 0, QApplication::UnicodeUTF8));
        m_print_toolBar->setWindowTitle(QApplication::translate("composerBase", "toolBar_2", 0, QApplication::UnicodeUTF8));
        toolBar_3->setWindowTitle(QApplication::translate("composerBase", "toolBar_3", 0, QApplication::UnicodeUTF8));
        m_ItemTools_toolBar->setWindowTitle(QApplication::translate("composerBase", "toolBar_4", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class composerBase: public Ui_composerBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QGIS_DEV_COMPOSERBASE_H
