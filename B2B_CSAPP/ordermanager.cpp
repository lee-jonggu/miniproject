#include "ordermanager.h"
#include "itemmanager.h"
#include "order.h"
#include "ui_ordermanager.h"

#include <QTime>

OrderManager::OrderManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OrderManager)
{
    ui->setupUi(this);

    QList<int> sizes;
    sizes << 450 << 630;
    ui->splitter->setSizes(sizes);
}

OrderManager::~OrderManager()
{
    delete ui;
}

int OrderManager::makeId( )
{
    if(orderList.size( ) == 0) {
        return 1;
    } else {
        auto id = orderList.lastKey();
        return ++id;
    }
}

void OrderManager::showClientData(Client* item)
{
    QString name, address, phoneNum, type;
    int id;
    id = item->id();
    name = item->getName();
    address = item->getAddress();
    phoneNum = item->getPhoneNum();
    type = item->getType();
    if(name.length()) {
        Client* c = new Client(id, name, address, phoneNum, type);
        ui->clientTreeWidget->addTopLevelItem(c);
    }
}

void OrderManager::on_clientLineEdit_returnPressed()
{
    ui->clientTreeWidget->clear();

    int i = ui->clientComboBox->currentIndex();

    if (i == 0) {
        int id = ui->clientLineEdit->text().toInt();
        emit clientDataSent(id);
    }
    else if (i==1) {
        QString name = ui->clientLineEdit->text();
        emit clientDataSent(name);
    }
    else if (i==2) {
        QString address = ui->clientLineEdit->text();
        emit clientDataSent(address);
    }
    else if (i==3) {
        QString type = ui->clientLineEdit->text();
        emit clientDataSent(type);
    }
}

void OrderManager::showItemData(Item* item)
{
    QString name, categori, color, stock, price;
    int id;
    id = item->id();
    name = item->getName();
    categori = item->getCategori();
    color = item->getColor();
    stock = item->getStock();
    price = item->getPrice();
    if(name.length()) {
        Item* c = new Item(id, name, categori, color, stock, price);
        ui->itemTreeWidget->addTopLevelItem(c);
    }
}


void OrderManager::on_itemLineEdit_returnPressed()
{
    ui->itemTreeWidget->clear();

    int i = ui->itemComboBox->currentIndex();

    if (i == 0) {
        int id = ui->itemLineEdit->text().toInt();
        emit itemDataSent(id);
    }
    else if (i==1) {
        QString name = ui->itemLineEdit->text();
        emit itemDataSent(name);
    }
    else if (i==2) {
        QString categori = ui->itemLineEdit->text();
        emit itemDataSent(categori);
    }
    else if (i==3) {
        QString color = ui->itemLineEdit->text();
        emit itemDataSent(color);
    }
}


void OrderManager::on_clientTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);

    QTreeWidgetItem* c = ui->clientTreeWidget->currentItem();
    if(c != nullptr)
    {
        ui->orderClientIdlineEdit->setText(item->text(0));
    }
}


void OrderManager::on_itemTreeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column);

    QTreeWidgetItem* c = ui->itemTreeWidget->currentItem();
    if(c != nullptr)
    {
        ui->orderItemIdlineEdit->setText(item->text(0));
        ui->orderPricelineEdit->setText(item->text(5));
    }
}


void OrderManager::on_orderQuantitylineEdit_returnPressed()
{
    int q = ui->orderQuantitylineEdit->text().toInt();

    int amount = ui->orderPricelineEdit->text().toInt() * q;

    ui->orderAmountlineEdit->setText(QString::number(amount));
}


void OrderManager::on_orderPushButton_clicked()
{
    QTreeWidgetItem* row = new QTreeWidgetItem(ui->orderTreeWidget);

    // Order Number
    int orderNum = makeId();

    // Date
    QString date = QTime::currentTime().toString();

    // Client Name, Item Name
    int c_id = ui->orderClientIdlineEdit->text().toInt();
    int i_id = ui->orderItemIdlineEdit->text().toInt();
    emit clientNameDataSent(c_id,row);
    emit itemNameDataSent(i_id,row);

    // Qunatity
    int q = ui->orderQuantitylineEdit->text().toInt();

    // Amount
    int amount = ui->orderPricelineEdit->text().toInt() * q;

    Order* order = new Order(orderNum,c_id,i_id,q);
    orderList.insert(orderNum,order);

    row->setText(0,QString::number(orderNum));
    row->setText(1,date);
    row->setText(4,QString::number(q));
    row->setText(5,QString::number(amount));
}

void OrderManager::showClientNameData(Client* client,QTreeWidgetItem* row)
{
    QString name, address, phoneNum, type;
    int id;
    id = client->id();
    name = client->getName();
    address = client->getAddress();
    phoneNum = client->getPhoneNum();
    type = client->getType();
    if(name.length()) {
        row->setText(2,name);
    }
}

void OrderManager::showItemNameData(Item* item,QTreeWidgetItem* row)
{
    QString name, categori, color, stock, price;
    int id;
    id = item->id();
    name = item->getName();
    categori = item->getCategori();
    color = item->getColor();
    stock = item->getStock();
    price = item->getPrice();
    if(name.length()) {
        row->setText(3,name);
    }
}

void OrderManager::on_SearchPushButton_clicked()
{
    ui->searchTreeWidget->clear();

    int i = ui->SearchComboBox->currentIndex();
    auto flag = (i)? Qt::MatchCaseSensitive|Qt::MatchContains
                   : Qt::MatchCaseSensitive;
    {
        auto items = ui->orderTreeWidget->findItems(ui->searchLineEdit->text(),flag,i);

        foreach(auto i, items)
        {
            Order* o = static_cast<Order*>(i);
            int orderNum = o->orderNum();
            ui->searchTreeWidget->addTopLevelItem(orderList[orderNum]);
        }
    }
}


void OrderManager::on_clearPushButton_clicked()
{
    ui->orderClientIdlineEdit->clear();
    ui->orderItemIdlineEdit->clear();
    ui->orderQuantitylineEdit->clear();
    ui->orderPricelineEdit->clear();
    ui->orderAmountlineEdit->clear();
}

