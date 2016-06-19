#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Enable the event Filter
    qApp->installEventFilter(this);

    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setBackgroundBrush(QPixmap(":/bg.jpg").scaled(960, 540));

    // Initionize
    numBird = 4;
    flying = false;
    flag = true;
    end = false;
    score = 0;
    ui->label_Score->setFont(QFont("Courier New", 25, QFont::Bold));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *)
{
    // Setting the QGraphicsScene
    scene = new QGraphicsScene(0,0,width(),ui->graphicsView->height());
    ui->graphicsView->setScene(scene);

    // Create world
    world = new b2World(b2Vec2(0.0f, -9.8f));

    // Setting Size
    GameItem::setGlobalSize(QSizeF(32,18),size());

    // Create ground (You can edit here)
    itemList.push_back(new Land(16,1.5,32,3,QPixmap(":/ground.png").scaled(width(),height()/6.0),world,scene));

    // Create bird (You can edit here)
    birdR = new Bird(0.0f,5.0f,0.27f,&timer,QPixmap( ":/bird.png").scaled(height()/9.0,height()/9.0),world,scene);
    birdY = new Bird(2.0f,5.0f,0.27f,&timer,QPixmap(":/birdY.png").scaled(height()/9.0,height()/9.0),world,scene);
    birdG = new Bird(4.0f,5.0f,0.27f,&timer,QPixmap(":/birdG.png").scaled(height()/9.0,height()/9.0),world,scene);
    birdB = new Bird(6.0f,5.0f,0.27f,&timer,QPixmap(":/birdB.png").scaled(height()/9.0,height()/9.0),world,scene);

    pig1 = new Bird(20.0f,5.0f,0.27f,&timer,QPixmap(":/pig.png").scaled(height()/9.0,height()/9.0),world,scene);
    //pig2 = new Bird(20.0f,30.0f,0.27f,&timer,QPixmap(":/pig.png").scaled(height()/9.0,height()/9.0),world,scene);
    block1 = new Block(15.0f, 5.0f, 0.27f, 1.89f, &timer, QPixmap(":/block.png").scaled(0.27*125, 1.89*50),world,scene);
    block2 = new Block(25.0f, 5.0f, 0.27f, 1.89f, &timer, QPixmap(":/block.png").scaled(0.27*125, 1.89*50),world,scene);
    block3 = new Block(20.0f, 10.0f, 16.2f, 0.54f, &timer, QPixmap(":/blockT.png").scaled(16.2*32,0.27*125),world,scene);
    //block4 = new Block(17.5f, 20.0f, 0.27f, 1.89f, &timer, QPixmap(":/block.png").scaled(0.27*125, 1.89*50),world,scene);

    Reset = new QPushButton(this);
    Reset->setGeometry(30,10,125,125);
    connect(Reset, SIGNAL (released()), this, SLOT (on_Reset_clicked()));
    Reset->setObjectName("Reset");
    Reset->show();
    Reset->setStyleSheet("#Reset{border-image:url(:/restart.png)}");


    Quit = new QPushButton(this);
    Quit->setGeometry(160,10,125,125);
    connect(Quit, SIGNAL (released()), this, SLOT (close()));
    Quit->setObjectName("Quit");
    Quit->show();
    Quit->setStyleSheet("#Quit{border-image:url(:/end.png)}");

    // Timer
    connect(&timer,SIGNAL(timeout()),this,SLOT(tick()));
    connect(&timer,SIGNAL(timeout()),this,SLOT(boundaryCheck()));
    connect(this,SIGNAL(quitGame()),this,SLOT(QUITSLOT()));
    timer.start(100/6);

}

bool MainWindow::eventFilter(QObject *, QEvent *event)
{
    // Hint: Notice the Number of every event!
    QMouseEvent *mouseEvent = static_cast <QMouseEvent*> (event);

    if(event->type() == QEvent::MouseButtonPress)
    {
        /* TODO : add your code here */
        //std::cout << "Press !" << std::endl ;

        if (flag == true)
        {
            if (flying == false)
            {
                xBegin = mouseEvent->x();
                yBegin = mouseEvent->y();
            }
        }flag = !flag;
    }
    if(event->type() == QEvent::MouseMove)
    {
        /* TODO : add your code here */
        //std::cout << "Move !" << std::endl;
    }
    if(event->type() == QEvent::MouseButtonRelease)     //two times signal
    {
        /* TODO : add your code here */
        //std::cout << "Release !" << std::endl ;
        if (flag == true)
        {
            if (flying == false)
            {
                xEnd = mouseEvent->x();
                yEnd = mouseEvent->y();

                if (numBird==4)    birdB->setLinearVelocity(b2Vec2((xBegin-xEnd)/10, (yBegin-yEnd)/10));
                if (numBird==3)    birdG->setLinearVelocity(b2Vec2((xBegin-xEnd)/10, (yBegin-yEnd)/10));
                if (numBird==2)    birdY->setLinearVelocity(b2Vec2((xBegin-xEnd)/10, (yBegin-yEnd)/10));
                if (numBird==1)    birdR->setLinearVelocity(b2Vec2((xBegin-xEnd)/10, (yBegin-yEnd)/10));

                numBird -= 1;
                flying = true;
            }
            else
            {
                if (numBird==3)   birdB->skill_blue();
                if (numBird==2)   birdG->skill_green();
                if (numBird==1)   birdY->skill_yellow();
                if (numBird==0)   birdR->skill_red();

                flying = false;
            }
        }flag = !flag;
    }
    return false;
}

void MainWindow::closeEvent(QCloseEvent *)
{
    // Close event
    emit quitGame();
}

void MainWindow::tick()
{
    world->Step(1.0/60.0,6,2);
    scene->update();

    if (numBird==4) score=0;

    score += block1->getScore();
    score += block2->getScore();
    score += block3->getScore();

    score += pig1->getScore()*10;

    QString Score = "Score:";
    Score += QString::number(score);
    ui->label_Score->setText(Score);
}

void MainWindow::QUITSLOT()
{
    // For debug
    std::cout << "Quit Game Signal receive !" << std::endl ;
}

void MainWindow::on_Reset_clicked()
{
    MainWindow *c = new MainWindow();
    c->show();
    hide();
}


void MainWindow::boundaryCheck()
{
    if (block1->checkX() || block1->checkY()) block1->change();
    if (block2->checkX() || block2->checkY()) block2->change();
    if (block3->checkX() || block3->checkY()) block3->change();
}
