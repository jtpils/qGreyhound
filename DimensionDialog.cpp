#include "DimensionDialog.h"

DimensionDialog::DimensionDialog(const std::vector<QString>& names, QWidget *parent) : QDialog(parent)
{
	setWindowTitle("Choose dimensions");

	createListWidget(names);
	createOtherWidgets();
	createLayout();
	createConnections();
}

void DimensionDialog::createListWidget(const std::vector<QString>& names) {
	widget = new QListWidget;

	for (const QString& name : names) {
		widget->addItem(name);
	}

	QListWidgetItem* item = 0;
	for (int i(0); i < widget->count(); ++i) {
		item = widget->item(i);
		item->setFlags(item->flags() | Qt::ItemIsUserCheckable);

		if (item->text() == "X" || item->text() == "Y" || item->text() == "Z") {
			item->setCheckState(Qt::Checked);
		}
		else {
			item->setCheckState(Qt::Unchecked);
		}
	}
}

void DimensionDialog::createOtherWidgets() {
	viewBox = new QGroupBox(tr("Required components"));
	buttonBox = new QDialogButtonBox;
	closeButton = buttonBox->addButton(QDialogButtonBox::Ok);
}

void DimensionDialog::createLayout() {
	QVBoxLayout* viewLayout = new QVBoxLayout;
	viewLayout->addWidget(widget);
	viewBox->setLayout(viewLayout);

	QHBoxLayout* horizontalLayout = new QHBoxLayout;
	horizontalLayout->addWidget(buttonBox);

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addWidget(viewBox);
	mainLayout->addLayout(horizontalLayout);

	setLayout(mainLayout);
}

void DimensionDialog::createConnections() {
	QObject::connect(widget, SIGNAL(itemChanged(QListWidgetItem*)),
		this, SLOT(highlightChecked(QListWidgetItem*)));
	QObject::connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
}

void DimensionDialog::highlightChecked(QListWidgetItem *item) {
	if (item->checkState() == Qt::Checked)
		item->setBackgroundColor(QColor("#ffffb2"));
	else
		item->setBackgroundColor(QColor("#ffffff"));
}


std::vector<QString> DimensionDialog::checked_dimensions() {
	std::vector<QString> checked_items;

	for (int i(0); i < widget->count(); ++i) {
		QListWidgetItem *item = widget->item(i);
		if (item->checkState() == Qt::Checked) {
			checked_items.push_back(item->text());
		}
	}
	return checked_items;
}