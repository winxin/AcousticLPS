#include "qenvironementeditor.h"
#include "ui_qenvironementeditor.h"
#include <model/qenvironement.h>
#include <QMessageBox>
#include <QUuid>
#include <QDebug>

QEnvironementEditor::QEnvironementEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QEnvironementEditor),
    _sourceModel(new BeaconListModel(parent)),
    _filterModel(new BeaconFilterModel(parent))
{
    ui->setupUi(this);


    filterModel()->setSourceModel(sourceModel());
    tableList()->resizeColumnsToContents();
    tableList()->setModel(filterModel());
    tableList()->horizontalHeader()->setStretchLastSection(true);
    tableList()->horizontalHeader()->setStretchLastSection(true);
    tableList()->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tableList()->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableList()->setSelectionMode(QAbstractItemView::SingleSelection);
    tableList()->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableList()->setShowGrid(false);

    connect(ui->stackedWidget, &QStackedWidget::currentChanged, [&](int row) {
        ui->backButton->setEnabled(row == BeaconsEditor);
        ui->nextButton->setEnabled(row == EnvironementEditor);
    });

    connect(ui->nextButton, &QToolButton::released, [&](){
        ui->stackedWidget->setCurrentIndex(BeaconsEditor);
    });

    connect(ui->backButton,  &QToolButton::released, [&](){
        ui->stackedWidget->setCurrentIndex(EnvironementEditor);
    });

    connect(ui->uuidGenerator, &QToolButton::released, [&]() {
        const QUuid uuid = QUuid::createUuid();
        ui->beaconUUID->setText(uuid.toString());
    });

    connect(ui->addButton, &QToolButton::released, [&]() {
        const QString name = ui->beaconName->text();
        if (name.isEmpty()) {
            QMessageBox::information(this, "Empty name", "Please insert a valid name for the beacon");
            return;
        }

        QString point = ui->beaconPosition->text();
        point = point.remove('(');
        point = point.remove(')');
        const QStringList values = point.split(',');
        Position position;
        const int size = values.size();
        for (int i=0; i<size; i++) {
            position(i) = QString(values.at(i)).toFloat();
        }

        const QString uuidString = ui->beaconUUID->text();
        const QUuid uuid(uuidString);

        const QBeacon beacon = QEnvironement::instance()->addBeacon();
        beacon->setName(name);
        beacon->setPosition(position);
        beacon->setUniversalUniqueIdentifier(uuid);
        filterModel()->invalidate();

        ui->beaconName->clear();
        ui->beaconPosition->clear();
        ui->beaconUUID->clear();

        ui->nameEdit->setFocus();
    });

    ui->stackedWidget->setCurrentIndex(EnvironementEditor);
}

QEnvironementEditor::~QEnvironementEditor() {
    delete ui;
    delete _sourceModel;
    delete _filterModel;
}

void QEnvironementEditor::setCurrentStep(QEnvironementEditor::Steps step) {
    ui->stackedWidget->setCurrentIndex(step);
}

QTableView* QEnvironementEditor::tableList() const {
    return ui->beaconList;
}

