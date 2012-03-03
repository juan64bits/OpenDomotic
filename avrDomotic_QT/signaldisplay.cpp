#include "signaldisplay.h"

SignalDisplay::SignalDisplay (QWidget *parent) : QWidget(parent)
{
    colorTrace1 = Qt::red;
    secsPerDiv = 1.0/600.0;
    voltsPerDiv = 20;
    setPointsPerPlot(10);
    traceWidth=1;
    nx = 10;
    ny = 10;
    autoScale=true;
    useSuffixs=true;
    showTextScale=true;
    showBars = true;
    showTraces = false;
    timeLine = new QTimeLine(10,0);
    timeLine->setCurveShape(QTimeLine::EaseInOutCurve);
}

void SignalDisplay::addPoint( int value1, int time)
{
    if(time==0 || value1==0)
    {
        waves.push_back(value1);
        waves.erase(waves.begin());
        repaint();
    }
    else
    {
        if(waves.size()>0)
        {
            waves.pop_back();
            waves.push_back(timeLine->endFrame());
        }

        timeLine->stop();

        waves.push_back(value1);
        waves.erase(waves.begin());

        timeLine->setDuration(time);
        timeLine->setFrameRange(0, value1);
        connect(timeLine, SIGNAL(frameChanged(int)), this, SLOT(setLastValue(int)));
        timeLine->start();        
    }
}

void SignalDisplay::setLastValue(int value)
{    
    if(waves.size()>0)
    {
        waves.pop_back();
        waves.push_back(value);        
        repaint();
    }    
}

void SignalDisplay::setPointsPerPlot(int value)
{
    pointsPerPlot = value+1;
    waves.clear();
    for(int i = 0; i<pointsPerPlot; i++){
        waves.push_back(0);
    }
}

void SignalDisplay::drawGrid(QPainter &p, QColor colorGrid, int w, int h)
{
    QPen pen(Qt::DotLine);
    pen.setColor(colorGrid);
    p.setPen(pen);

    for (int ix= 0; ix<nx; ix++){
		int x = ix*w/nx;
        p.drawLine(x,0,x,h);

	}
    for (int iy = 0; iy < ny+1; iy++){
		int y = iy*h/ny;
		p.drawLine(0,y,w,y);

        if(showTextScale)
        {
            QString text = QString::number((ny-iy)*voltsPerDiv);
            if(useSuffixs)
            {
                int number = (ny-iy)*voltsPerDiv;

                if(number >= 1000000)
                    text = QString::number((ny-iy)*voltsPerDiv/1000000) + "M";
                if(number >= 1000)
                    text = QString::number((ny-iy)*voltsPerDiv/1000) + "K";
            }
            p.drawText(0,y,text);
        }
	}
}
void SignalDisplay::paintEvent(QPaintEvent *event){
	QPainter painter(this);
	w = width();
	h = height();
	ox = w;
	oy = h;

    for (int iy = 0; iy < h; iy++){
        painter.fillRect(0,iy,w,10,QColor(0,0,(unsigned char)(255-iy*255/h)));
    }

    QPoint points[pointsPerPlot];
    int max=0;
    for (std::vector<int>::iterator sample = waves.begin(); sample< waves.end();sample ++)
        if(*sample>max)
            max = *sample;

    if(autoScale)    
        ny = (max / voltsPerDiv)+1;

    int j = 0;
    for (std::vector<int>::iterator sample = waves.begin(); sample< waves.end();sample ++)
    {
        points[j] = QPoint(j*w/10/60.0/(pointsPerPlot)/secsPerDiv , oy-*sample*h/voltsPerDiv/ny);
        j++;
    }

    if(!(waves.size()>0))
        return;   

    if(showBars)
    {
        for(int i = 0; i < 	pointsPerPlot; i++)
        {
            painter.fillRect(points[i].x()-w/pointsPerPlot/3,points[i].y(),
                             w/pointsPerPlot-w/pointsPerPlot/3,h-points[i].y(), colorTrace1);
        }
    }

    drawGrid(painter, Qt::lightGray,w,h);

    if(showTraces)
    {
        QPen pen(Qt::SolidLine);
        pen.setColor(colorTrace1);
        pen.setWidth(traceWidth);
        painter.setPen(pen);
        painter.drawPolyline(points,pointsPerPlot);
    }    

}
//EOF
