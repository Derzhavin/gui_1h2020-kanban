#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H


class AnimationManager
{
public:
    AnimationManager();
};

//void highlight(QWidget *widget) {
//    QTimeLine *timeLine = new QTimeLine(1000, widget);

//    timeLine->setFrameRange(0, 255);

//    QObject::connect(timeLine, &QTimeLine::frameChanged, [=](int frame){
//        QPalette palette = widget->palette();
//        palette.setBrush(QPalette::Base, QBrush(QColor(frame, frame, frame)));
//       widget->setPalette(palette);
//    });

#endif // ANIMATIONMANAGER_H
