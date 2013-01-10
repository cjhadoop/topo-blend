#pragma once

#include <QGraphicsObject>
#include "TopoBlender.h"

// red, orange, yellow, green, blue
// pink, purpule, portage, lacoste, corn
static QColor TaskColors[] = { QColor(255,97,121),  QColor(107,255,135), 
	QColor(255,219,88), QColor(255,165,107) , QColor(104,126,255),
	QColor(242,5,135), QColor(113,53,242), QColor(138,109,242), QColor(3,166,60), QColor(242,203,5)};

static QString TaskNames[] = { "SHRINK", "MORPH", "MERGE", "SPLIT", "GROW" };

class Task : public QGraphicsObject
{
public:
	enum TaskType{ SHRINK, MORPH, MERGE, SPLIT, GROW };

	Task( Structure::Graph * activeGraph, Structure::Graph * targetGraph, TaskType taskType, int ID );
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
	void mousePressEvent(QGraphicsSceneMouseEvent * event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
		
	// Prepare stage
	void prepare();
	void prepareGrowShrink();
	void prepareMorph();

	// Execution stage
	void execute( double t );
	void executeGrowShrink( double t );
	void executeMorph( double t );

	Structure::Node * node();

	QVector<Structure::Link *> getGoodEdges();
	QList<Structure::Link*> furthermostGoodEdges();

	// Helper functions
	void setupCurveDeformer(Structure::Curve* curve, Structure::Link* linkA, Structure::Link* linkB);

	// Task properties
	TaskType type;
	int arapIterations;

	Structure::Graph *active, *target;
	QMap<QString, QVariant> property;

	// Time related
	int start;
	int length;
	int currentTime;
	bool isReady;
	int taskID;

	void setLength(int newLength);
	void reset();
	bool stillWorking();
	bool isDone;
	int endTime();
	double localT( int globalTime );

	// Visual properties
	int width;
	int height;
	QColor mycolor;

	// Resizing variables
	bool isResizing;
	int resizeDir;
	QPointF clickPos, myOldPos;
	int myOldWidth;

	// DEBUG:
	std::vector<Vec3d> debugPoints, debugPoints2;
	void drawDebug();

protected:
	virtual QVariant itemChange ( GraphicsItemChange change, const QVariant & value );

};