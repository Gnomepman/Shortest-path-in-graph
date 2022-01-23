#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QGraphicsTextItem"
#include "QListWidget"

//can control radius of vertices and distance between them
#define ellipseRadius 30
#define radius 120

//кисти, ручки
QBrush whiteBrush(Qt::white);

QPen bluepen(Qt::blue);
QPen redpen(Qt::red);
QPen graypen(Qt::gray);

QPen bluepenDot(Qt::blue);
QPen redpenDot(Qt::red);

std::vector <Connections> edges;
std::vector <int> usedVertices;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->spinBox->setMinimum(0);

    scene = new QGraphicsScene(this);
    scene->clearFocus();
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    graypen.setWidth(3);

    bluepen.setWidth(2);
    redpen.setWidth(2);

    bluepenDot.setWidth(2);
    redpenDot.setWidth(2);

    bluepen.setCapStyle(Qt::RoundCap);
    redpen.setCapStyle(Qt::RoundCap);

    bluepenDot.setStyle(Qt::DashLine);
    redpenDot.setStyle(Qt::DashLine);

    ui->spinBox_first->setMinimum(1);
    ui->spinBox_second->setMinimum(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintVerticies(int numberOfverticies){
    if (numberOfverticies==0)
        return;
    for (int i=1; i<=numberOfverticies; ++i){
        scene->addEllipse(radius*cos(i),radius*sin(i),ellipseRadius,ellipseRadius,graypen,whiteBrush);
        QGraphicsTextItem *text = scene->addText(QString::number(i));
        text->setPos(radius*cos(i)+7,radius*sin(i)+3);
    }
};


void MainWindow::on_spinBox_valueChanged(int arg1)
{
    ui->tableSum->setRowCount(arg1);
    ui->tableSum->setColumnCount(arg1);

    ui->spinBox_first->setMaximum(arg1);
    ui->spinBox_second->setMaximum(arg1);

    for (int i=0;i<ui->tableSum->rowCount();++i){
        for (int j=0;j<ui->tableSum->columnCount();++j) {
            ui->tableSum->setColumnWidth(i,10);
            QTableWidgetItem *defaultCell = new QTableWidgetItem();
            defaultCell->setText(QString::fromStdString("0"));
            defaultCell->setTextAlignment(Qt::AlignHCenter);
            ui->tableSum->setItem(i,j,defaultCell);
        }
    }
}


void MainWindow::on_pushButton_clicked()
{
    ui->tableSum->clear();
    ui->spinBox->setValue(0);
    ui->labelResult->clear();
    scene->clear();
}


void MainWindow::on_pushButton_2_clicked()
{
    edges.clear();
    scene->clear();

    for(int i=0; i<ui->tableSum->rowCount(); ++i){
        for(int j=0; j<ui->tableSum->rowCount(); ++j){
            if(ui->tableSum->item(i,j)->text()!="0" && ui->tableSum->item(i,j)->text()!="∞"){
                Connections pair;
                pair.first=i+1;
                pair.second=j+1;
                edges.push_back(pair);
            }
        }
    }
    for (Connections item : edges) {
        if(item.first==item.second){
            scene->addEllipse(radius*cos(item.first)-5,radius*sin(item.second)-5,ellipseRadius+5,ellipseRadius+5,bluepenDot);
        }else{
            printArrowedEdge(item.first,item.second,bluepen);
        }
    }
    paintVerticies(ui->tableSum->columnCount());
}

void MainWindow::printArrowedEdge(int first, int second, QPen pen){
    float x1=radius*cos(first)+ellipseRadius/2,
            y1=radius*sin(first)+ellipseRadius/2,
            x2=radius*cos(second)+ellipseRadius/2,
            y2=radius*sin(second)+ellipseRadius/2;
    float x, y;
    float f1x2 , f1y2;
    float lons, angle;

    const float sharpness = 0.2;        // острота стрелки | 0.25

    scene->addLine(x1, y1, x2, y2, pen);

    x = x2 - x1;
    y = y2 - y1;

    lons = sqrt(x*x + y*y) / 5;     // длина лепестков % от длины стрелки | 7
    angle = atan2(y, x);            // угол наклона линии

    f1x2 = x2 - lons * cos(angle - sharpness);
    f1y2 = y2 - lons * sin(angle - sharpness);

    scene->addLine(x2, y2, f1x2, f1y2, pen);

    f1x2 = x2 - lons * cos(angle + sharpness);
    f1y2 = y2 - lons * sin(angle + sharpness);

    scene->addLine(x2, y2, f1x2, f1y2, pen);
}


void MainWindow::on_tableSum_cellClicked(int row, int column)
{
}

bool include(std::vector <int> myVector, int element){
    for (int i=0; i<(int)myVector.size(); ++i){
        if(myVector[i]==element){
            return true;
        }
    }
    return false;
}


void MainWindow::on_pushButton_3_clicked()
{

}

int min(int a, int b){
    return (a<b) ? a : b;
}


void MainWindow::on_pushButton_4_clicked()
{

}

//кратчайший путь из A в B (алгоритм Дейкстры)
//алгоритм ищет кратчайший путь от точки до всех точек сразу, тут мы на нужной вершине прерываем выполнение
void MainWindow::on_pushButton_5_clicked()
{
    usedVertices.clear();
    ui->labelResult->clear();
    //запоминаем размер матрицы
    int size = ui->tableSum->rowCount();
    //двумерный массив для расчётов
    int temp[size][size];
    //массив, чтобы знать, из какой вершины мы пришли (нужен для построения пути)
    int previous[size];
    //переменная для сравнения
    int comparison;
    //начальная вершина
    int begin = ui->spinBox_first->value() - 1;
    //массив для расчётов проинициализировать очень большим числом (в идеале - бесконечностью)
    for (int i = 0; i < size; ++i)
        temp[0][i] = INT_MAX;
    //массив previous для всех элементов проинициализировать begin
    for(int i = 0; i < size; ++i)
        previous[i] = begin;
    //двумерный массив на первой итерации в столбей begin записать 0
    temp[0][begin] = 0;
    //начальная вершина считается использованной
    usedVertices.push_back(begin);
    //сравнение начинается для begin
    comparison=begin;

    for (int i=1;i<=size;i++){
        for (int j=0;j<size;j++){
            //Если данная вершина не была использована
            if(!include(usedVertices,j)){
                if(ui->tableSum->item(comparison,j)->text().toInt()<=0){//Если ребра не существует или оно отрицательное
                     temp[i][j]=min(INT_MAX,temp[i-1][j]);//вписываем в двумерный массив очень большое число (в иделе массив)
                }else{//иначе
                    //вписать меньшее из двух чисел - предыдущего значения столбца или суммы двух чисел
                     temp[i][j]=min(ui->tableSum->item(comparison,j)->text().toInt()+temp[i-1][usedVertices.back()],temp[i-1][j]);
                }
            }
            //Если сумма оказалась меньше предыдущего значения - вписать в previous под индексом данной вершины вершину, откуда пришли
            if(!include(usedVertices,j) && ui->tableSum->item(comparison,j)->text().toInt()+temp[i-1][usedVertices.back()]<temp[i-1][j]){
                previous[j]=comparison;
            }

        }
        //поочерёдно сравнивая все элементы строки ищем минимальный путь
        int min = INT_MAX;
        for (int k=0;k<size;++k){
            if(include(usedVertices,k)){
                continue;
            }
            if(temp[i][k]<min){
                min = temp[i][k];
                comparison=k;
            }
        }
        //после всех операций минимальный путь для вершины comparison найден
        usedVertices.push_back(comparison);
        //Если оказывается, что comparison - конечная вершина
        if(comparison==ui->spinBox_second->value()-1){
            int search=previous[comparison];
            //Конкатенацией строим результат
            QString result;
            if (search==begin){
                result = "x" + QString::number(begin+1) + " - " + " x" + QString::number(comparison+1);
            }else{
                result += QString::number(comparison+1) + "x -";
                do{
                    result += QString::number(search+1) + "x -";
                    search = previous[search];
                }while(search!=begin);
                result += QString::number(begin+1) + "x -";
                std::reverse(result.begin(),result.end());
            }
            ui->labelResult->setText("Shortest path between x"
                                     + QString::number(begin+1)
                                     + " and x" + QString::number(ui->spinBox_second->value())
                                     + " = " + QString::number(min) + "\n" + result);
            return;
        }
    }
}

