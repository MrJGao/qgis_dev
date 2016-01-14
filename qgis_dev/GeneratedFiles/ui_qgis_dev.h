/********************************************************************************
** Form generated from reading UI file 'qgis_dev.ui'
**
** Created: Thu Jan 14 19:25:29 2016
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QGIS_DEV_H
#define UI_QGIS_DEV_H

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

class Ui_qgis_devClass
{
public:
    QAction *actionOpen_Project;
    QAction *actionSave_Project;
    QAction *actionExit;
    QAction *actionAdd_Vector;
    QAction *actionAdd_Raster;
    QAction *actionNew_Project;
    QAction *actionToggle_Overview;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuVector;
    QMenu *menuRaster;
    QMenu *menuView;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *qgis_devClass)
    {
        if (qgis_devClass->objectName().isEmpty())
            qgis_devClass->setObjectName(QString::fromUtf8("qgis_devClass"));
        qgis_devClass->setWindowModality(Qt::NonModal);
        qgis_devClass->resize(926, 580);
        qgis_devClass->setMouseTracking(true);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/icons/qgis-icon-60x60.png"), QSize(), QIcon::Normal, QIcon::Off);
        qgis_devClass->setWindowIcon(icon);
        qgis_devClass->setAutoFillBackground(true);
        qgis_devClass->setToolButtonStyle(Qt::ToolButtonIconOnly);
        qgis_devClass->setAnimated(true);
        qgis_devClass->setDocumentMode(false);
        actionOpen_Project = new QAction(qgis_devClass);
        actionOpen_Project->setObjectName(QString::fromUtf8("actionOpen_Project"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/themes/default/mActionFileOpen.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen_Project->setIcon(icon1);
        actionSave_Project = new QAction(qgis_devClass);
        actionSave_Project->setObjectName(QString::fromUtf8("actionSave_Project"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/themes/default/mActionFileSave.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionSave_Project->setIcon(icon2);
        actionExit = new QAction(qgis_devClass);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/themes/default/mActionFileExit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon3);
        actionAdd_Vector = new QAction(qgis_devClass);
        actionAdd_Vector->setObjectName(QString::fromUtf8("actionAdd_Vector"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/images/themes/default/mActionAddOgrLayer.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionAdd_Vector->setIcon(icon4);
        actionAdd_Raster = new QAction(qgis_devClass);
        actionAdd_Raster->setObjectName(QString::fromUtf8("actionAdd_Raster"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/images/themes/default/mActionAddRasterLayer.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionAdd_Raster->setIcon(icon5);
        actionNew_Project = new QAction(qgis_devClass);
        actionNew_Project->setObjectName(QString::fromUtf8("actionNew_Project"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/images/themes/default/mActionFileNew.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionNew_Project->setIcon(icon6);
        actionToggle_Overview = new QAction(qgis_devClass);
        actionToggle_Overview->setObjectName(QString::fromUtf8("actionToggle_Overview"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/images/themes/default/mActionInOverview.svg"), QSize(), QIcon::Normal, QIcon::Off);
        actionToggle_Overview->setIcon(icon7);
        centralWidget = new QWidget(qgis_devClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        qgis_devClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(qgis_devClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 926, 23));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuVector = new QMenu(menuBar);
        menuVector->setObjectName(QString::fromUtf8("menuVector"));
        menuRaster = new QMenu(menuBar);
        menuRaster->setObjectName(QString::fromUtf8("menuRaster"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        qgis_devClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(qgis_devClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        qgis_devClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(qgis_devClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        qgis_devClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuVector->menuAction());
        menuBar->addAction(menuRaster->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuFile->addAction(actionNew_Project);
        menuFile->addAction(actionOpen_Project);
        menuFile->addAction(actionSave_Project);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuVector->addAction(actionAdd_Vector);
        menuRaster->addAction(actionAdd_Raster);
        menuView->addAction(actionToggle_Overview);
        mainToolBar->addAction(actionNew_Project);
        mainToolBar->addAction(actionOpen_Project);
        mainToolBar->addAction(actionSave_Project);
        mainToolBar->addAction(actionAdd_Vector);
        mainToolBar->addAction(actionAdd_Raster);

        retranslateUi(qgis_devClass);

        QMetaObject::connectSlotsByName(qgis_devClass);
    } // setupUi

    void retranslateUi(QMainWindow *qgis_devClass)
    {
        qgis_devClass->setWindowTitle(QApplication::translate("qgis_devClass", "qgis_dev", 0, QApplication::UnicodeUTF8));
        actionOpen_Project->setText(QApplication::translate("qgis_devClass", "Open Project", 0, QApplication::UnicodeUTF8));
        actionSave_Project->setText(QApplication::translate("qgis_devClass", "Save Project", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("qgis_devClass", "Exit", 0, QApplication::UnicodeUTF8));
        actionAdd_Vector->setText(QApplication::translate("qgis_devClass", "Add Vector", 0, QApplication::UnicodeUTF8));
        actionAdd_Raster->setText(QApplication::translate("qgis_devClass", "Add Raster", 0, QApplication::UnicodeUTF8));
        actionNew_Project->setText(QApplication::translate("qgis_devClass", "New Project", 0, QApplication::UnicodeUTF8));
        actionToggle_Overview->setText(QApplication::translate("qgis_devClass", "Toggle Overview", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("qgis_devClass", "File", 0, QApplication::UnicodeUTF8));
        menuVector->setTitle(QApplication::translate("qgis_devClass", "Vector", 0, QApplication::UnicodeUTF8));
        menuRaster->setTitle(QApplication::translate("qgis_devClass", "Raster", 0, QApplication::UnicodeUTF8));
        menuView->setTitle(QApplication::translate("qgis_devClass", "View", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class qgis_devClass: public Ui_qgis_devClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QGIS_DEV_H
