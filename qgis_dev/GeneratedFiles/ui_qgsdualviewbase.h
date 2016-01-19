/********************************************************************************
** Form generated from reading UI file 'qgsdualviewbase.ui'
**
** Created: Tue Jan 19 20:08:45 2016
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QGSDUALVIEWBASE_H
#define UI_QGSDUALVIEWBASE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QFrame>
#include <QtGui/QGridLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QScrollArea>
#include <QtGui/QSplitter>
#include <QtGui/QStackedWidget>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "attributetable/qgsattributetableview.h"
#include "attributetable/qgsfeaturelistview.h"

QT_BEGIN_NAMESPACE

class Ui_QgsDualViewBase
{
public:
    QWidget *mPageTableView;
    QGridLayout *gridLayout_3;
    QgsAttributeTableView *mTableView;
    QWidget *mPageAttributeEditor;
    QAction *mActionExpressionPreview;
    QAction *mActionPreviewColumnsMenu;
    QGridLayout *gridLayout_2;
    QSplitter *mAttributeEditorViewSplitter;
    QWidget *listViewWdg;
    QVBoxLayout *verticalLayout_2;
    QToolButton *mFeatureListPreviewButton;
    QgsFeatureListView *mFeatureList;
    QFrame *mAttributeEditor;
    QGridLayout *gridLayout;
    QScrollArea *mAttributeEditorScrollArea;
    QWidget *mAttributeEditorLayout;
    QGridLayout *gridLayout_4;

    void setupUi(QStackedWidget *QgsDualViewBase)
    {
        if (QgsDualViewBase->objectName().isEmpty())
            QgsDualViewBase->setObjectName(QString::fromUtf8("QgsDualViewBase"));
        QgsDualViewBase->resize(495, 507);
        mPageTableView = new QWidget();
        mPageTableView->setObjectName(QString::fromUtf8("mPageTableView"));
        gridLayout_3 = new QGridLayout(mPageTableView);
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        mTableView = new QgsAttributeTableView(mPageTableView);
        mTableView->setObjectName(QString::fromUtf8("mTableView"));
        mTableView->setAlternatingRowColors(true);

        gridLayout_3->addWidget(mTableView, 0, 0, 1, 1);

        QgsDualViewBase->addWidget(mPageTableView);
        mPageAttributeEditor = new QWidget();
        mPageAttributeEditor->setObjectName(QString::fromUtf8("mPageAttributeEditor"));
        mActionExpressionPreview = new QAction(mPageAttributeEditor);
        mActionExpressionPreview->setObjectName(QString::fromUtf8("mActionExpressionPreview"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/themes/default/mActionFilter.png"), QSize(), QIcon::Normal, QIcon::Off);
        mActionExpressionPreview->setIcon(icon);
        mActionPreviewColumnsMenu = new QAction(mPageAttributeEditor);
        mActionPreviewColumnsMenu->setObjectName(QString::fromUtf8("mActionPreviewColumnsMenu"));
        gridLayout_2 = new QGridLayout(mPageAttributeEditor);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        mAttributeEditorViewSplitter = new QSplitter(mPageAttributeEditor);
        mAttributeEditorViewSplitter->setObjectName(QString::fromUtf8("mAttributeEditorViewSplitter"));
        mAttributeEditorViewSplitter->setOrientation(Qt::Horizontal);
        listViewWdg = new QWidget(mAttributeEditorViewSplitter);
        listViewWdg->setObjectName(QString::fromUtf8("listViewWdg"));
        verticalLayout_2 = new QVBoxLayout(listViewWdg);
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        mFeatureListPreviewButton = new QToolButton(listViewWdg);
        mFeatureListPreviewButton->setObjectName(QString::fromUtf8("mFeatureListPreviewButton"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(mFeatureListPreviewButton->sizePolicy().hasHeightForWidth());
        mFeatureListPreviewButton->setSizePolicy(sizePolicy);
        mFeatureListPreviewButton->setPopupMode(QToolButton::InstantPopup);
        mFeatureListPreviewButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        mFeatureListPreviewButton->setAutoRaise(true);
        mFeatureListPreviewButton->setArrowType(Qt::NoArrow);

        verticalLayout_2->addWidget(mFeatureListPreviewButton);

        mFeatureList = new QgsFeatureListView(listViewWdg);
        mFeatureList->setObjectName(QString::fromUtf8("mFeatureList"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(mFeatureList->sizePolicy().hasHeightForWidth());
        mFeatureList->setSizePolicy(sizePolicy1);

        verticalLayout_2->addWidget(mFeatureList);

        mAttributeEditorViewSplitter->addWidget(listViewWdg);
        mAttributeEditor = new QFrame(mAttributeEditorViewSplitter);
        mAttributeEditor->setObjectName(QString::fromUtf8("mAttributeEditor"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(1);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(mAttributeEditor->sizePolicy().hasHeightForWidth());
        mAttributeEditor->setSizePolicy(sizePolicy2);
        mAttributeEditor->setFrameShape(QFrame::StyledPanel);
        mAttributeEditor->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(mAttributeEditor);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        mAttributeEditorScrollArea = new QScrollArea(mAttributeEditor);
        mAttributeEditorScrollArea->setObjectName(QString::fromUtf8("mAttributeEditorScrollArea"));
        mAttributeEditorScrollArea->setWidgetResizable(true);
        mAttributeEditorLayout = new QWidget();
        mAttributeEditorLayout->setObjectName(QString::fromUtf8("mAttributeEditorLayout"));
        mAttributeEditorLayout->setGeometry(QRect(0, 0, 98, 28));
        gridLayout_4 = new QGridLayout(mAttributeEditorLayout);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        mAttributeEditorScrollArea->setWidget(mAttributeEditorLayout);

        gridLayout->addWidget(mAttributeEditorScrollArea, 0, 0, 1, 1);

        mAttributeEditorViewSplitter->addWidget(mAttributeEditor);

        gridLayout_2->addWidget(mAttributeEditorViewSplitter, 0, 0, 1, 1);

        QgsDualViewBase->addWidget(mPageAttributeEditor);

        retranslateUi(QgsDualViewBase);

        QgsDualViewBase->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(QgsDualViewBase);
    } // setupUi

    void retranslateUi(QStackedWidget *QgsDualViewBase)
    {
        mActionExpressionPreview->setText(QApplication::translate("QgsDualViewBase", "Expression", 0, QApplication::UnicodeUTF8));
        mActionPreviewColumnsMenu->setText(QApplication::translate("QgsDualViewBase", "Column Preview", 0, QApplication::UnicodeUTF8));
        mFeatureListPreviewButton->setText(QApplication::translate("QgsDualViewBase", "...", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(QgsDualViewBase);
    } // retranslateUi

};

namespace Ui {
    class QgsDualViewBase: public Ui_QgsDualViewBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QGSDUALVIEWBASE_H
