#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    plane = QPixmap::fromImage(QImage(":/plane"));
    intruder = QPixmap::fromImage(QImage(":/rhombus"));
    scale=0.3;
}

void Widget::paintEvent(QPaintEvent *event)
{

}

void Widget::resizeEvent(QResizeEvent *event){

    totalWidth = ui->graphicsView->width();
    totalHeight = ui->graphicsView->height();

}

void Widget::setup(){


    QBrush greyBrush(Qt::gray);
    QBrush blackBrush(Qt::black);
    QPen whitePen(Qt::white);


    intr = scene->addPixmap(intruder);
    ui->graphicsView->setScene(scene);
    whitePen.setWidth(10);
    scene->setBackgroundBrush(greyBrush);
    radius=totalHeight*0.9/2;
    scene->addEllipse(totalWidth/2-radius,totalHeight/2-radius,radius*2,radius*2,whitePen,greyBrush);
    pln = scene->addPixmap(plane);
    pln->setScale(scale);
    pln->setPos(totalWidth/2-plane.width()*(scale/2),totalHeight/2+plane.height()*scale*1.5);

    Pointer *point = new Pointer(totalWidth,totalHeight,radius);
    scene->addItem(point);
}

void Widget::setupListener(int portNum)
{
    broadcaster = new Broadcaster(portNum);
    scene_manager=new SceneManager(scene,broadcaster);

    time = new QTimer(this);
    QThread* listener = new QThread;
    scene_manager->moveToThread(listener);
    broadcaster->moveToThread(listener);

    connect(listener,SIGNAL(started()),broadcaster,SLOT(listenBuffer()));
    connect(broadcaster,SIGNAL(messageReceived(char*)),scene_manager,SLOT(updateScene(char*)));
    connect(time,SIGNAL(timeout()),scene,SLOT(advance()));
    time->start(10);
    listener->start();
}







Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    //Eu depois limpo isto
    //Mas limpa mesmo Gui isto parece o teu quarto
    if(ui->lineEdit->text().size()==0)
        return;
    else{
        setup();
        setupListener(ui->lineEdit->text().toInt());
        ui->pushButton->setVisible(false);
        ui->label->setVisible(false);
        ui->lineEdit->setVisible(false);
    }

}


