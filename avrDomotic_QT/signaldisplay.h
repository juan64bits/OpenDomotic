#ifndef SIGNALDISPLAY_H
#define SIGNALDISPLAY_H

#include <QWidget>
#include <cmath>
#include <QtGui>
#include <QtCore>


class SignalDisplay : public QWidget
{
Q_OBJECT
public:
    SignalDisplay(QWidget *parent = 0);

    void addPoint( int value1,int time = 0) ;
    void clearWaves(){waves.clear(); timeLine->stop();}
	void setSecsPerDiv( float value ){ secsPerDiv = fabs(value);}
	void setVoltsPerDiv( float value ){ voltsPerDiv = fabs(value);}
	float getSecsPerDiv(){ return secsPerDiv; }
	void  setPointsPerPlot(int value);
	void setColorTrace1(QColor color){colorTrace1=color;}
    void setTraceWidth(int value) {traceWidth = value;}
    void setGridValue(int x, int y, bool a = false) {nx=x+1; ny=y+1; autoScale=a;}
    void setShow(bool t, bool b) {showBars=b; showTraces=t;}

protected:
	void paintEvent(QPaintEvent *event);

private slots:
    void setLastValue(int value);

private:
    void drawGrid(QPainter &p, QColor colorGrid, int w, int h);
    std::vector<int> waves;    
	float voltsPerDiv;
	float secsPerDiv;
    QColor colorTrace1;    
    int w, h, ox, oy, nx, ny;
	int pointsPerPlot;
    int traceWidth;

    bool useSuffixs;
    bool showTextScale;
    bool autoScale;
    bool showBars;
    bool showTraces;    
    QTimeLine *timeLine;
};

#endif // SIGNALDISPLAY_H
