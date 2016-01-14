/********************************************************************************
** Form generated from reading UI file 'qgis_dev_composerBase.ui'
**
** Created: Thu Jan 14 22:52:09 2016
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
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menuProject;
    QStatusBar *statusbar;
    QToolBar *mProjectToolBar;
    QToolBar *toolBar;
    QToolBar *toolBar_2;

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
        centralwidget = new QWidget(composerBase);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        composerBase->setCentralWidget(centralwidget);
        menubar = new QMenuBar(composerBase);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 23));
        menuProject = new QMenu(menubar);
        menuProject->setObjectName(QString::fromUtf8("menuProject"));
        composerBase->setMenuBar(menubar);
        statusbar = new QStatusBar(composerBase);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        composerBase->setStatusBar(statusbar);
        mProjectToolBar = new QToolBar(composerBase);
        mProjectToolBar->setObjectName(QString::fromUtf8("mProjectToolBar"));
        composerBase->addToolBar(Qt::TopToolBarArea, mProjectToolBar);
        toolBar = new QToolBar(composerBase);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        composerBase->addToolBar(Qt::LeftToolBarArea, toolBar);
        toolBar_2 = new QToolBar(composerBase);
        toolBar_2->setObjectName(QString::fromUtf8("toolBar_2"));
        composerBase->addToolBar(Qt::TopToolBarArea, toolBar_2);

        menubar->addAction(menuProject->menuAction());
        menuProject->addAction(actionNew_Project);
        menuProject->addAction(actionSave_Project);
        mProjectToolBar->addAction(actionNew_Project);
        mProjectToolBar->addAction(actionSave_Project);
        toolBar->addAction(actionZoomAll);
        toolBar->addAction(actionZoomActual);
        toolBar->addAction(actionZoomIn);
        toolBar->addAction(actionZoomOut);
        toolBar_2->addAction(actionPrint);
        toolBar_2->addAction(actionExportAsImage);
        toolBar_2->addAction(actionExportAsSvg);

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
        menuProject->setTitle(QApplication::translate("composerBase", "Project", 0, QApplication::UnicodeUTF8));
        mProjectToolBar->setWindowTitle(QApplication::translate("composerBase", "toolBar", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("composerBase", "toolBar", 0, QApplication::UnicodeUTF8));
        toolBar_2->setWindowTitle(QApplication::translate("composerBase", "toolBar_2", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class composerBase: public Ui_composerBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QGIS_DEV_COMPOSERBASE_H
