/********************************************************************************
** Form generated from reading UI file 'qgis_devattrtabledialog.ui'
**
** Created: Thu Dec 31 15:09:12 2015
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QGIS_DEVATTRTABLEDIALOG_H
#define UI_QGIS_DEVATTRTABLEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_qgis_devattrtableDialog
{
public:
    QGridLayout *gridLayout;
    QWidget *mainWidget;

    void setupUi(QDialog *qgis_devattrtableDialog)
    {
        if (qgis_devattrtableDialog->objectName().isEmpty())
            qgis_devattrtableDialog->setObjectName(QString::fromUtf8("qgis_devattrtableDialog"));
        qgis_devattrtableDialog->resize(524, 397);
        gridLayout = new QGridLayout(qgis_devattrtableDialog);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        mainWidget = new QWidget(qgis_devattrtableDialog);
        mainWidget->setObjectName(QString::fromUtf8("mainWidget"));

        gridLayout->addWidget(mainWidget, 0, 0, 1, 1);


        retranslateUi(qgis_devattrtableDialog);

        QMetaObject::connectSlotsByName(qgis_devattrtableDialog);
    } // setupUi

    void retranslateUi(QDialog *qgis_devattrtableDialog)
    {
        qgis_devattrtableDialog->setWindowTitle(QApplication::translate("qgis_devattrtableDialog", "qgis_devattrtableDialog", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class qgis_devattrtableDialog: public Ui_qgis_devattrtableDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QGIS_DEVATTRTABLEDIALOG_H
