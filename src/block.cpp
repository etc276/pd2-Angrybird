#include "block.h"

Block::Block(float x, float y, float w, float d, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene):GameItem(world)
{
    // Set pixmap
    g_pixmap.setPixmap(pixmap);
    g_pixmap.setTransformOriginPoint(g_pixmap.boundingRect().width()/2,g_pixmap.boundingRect().height()/2);
    g_size = QSize(w,d);

    // Create Body
    b2BodyDef bodydef;
    bodydef.type = b2_dynamicBody;
    bodydef.bullet = true;
    bodydef.position.Set(x,y);
    bodydef.userData = this;
    g_body = world->CreateBody(&bodydef);

    b2PolygonShape bodyshape;
    bodyshape.SetAsBox(w/2,d/2);

    b2FixtureDef fixturedef;
    fixturedef.shape = &bodyshape;
    fixturedef.density = BLOCK_DENSITY;
    fixturedef.friction = BLOCK_FRICTION;
    fixturedef.restitution = BLOCK_RESTITUTION;
    g_body->SetAngularDamping(3);
    g_body->CreateFixture(&fixturedef);

    // Bound timer
    connect(timer, SIGNAL(timeout()), this,SLOT(paint()));

    scene->addItem(&g_pixmap);
}

int Block::getScore()
{
    double Vx = g_body->GetLinearVelocity().x;
    double Vy = g_body->GetLinearVelocity().y;
    return (int)sqrt(Vx*Vx + Vy*Vy);
}

bool Block::checkX()
{
    return g_body->GetPosition().x>40 || g_body->GetPosition().x<0;
}

bool Block::checkY()
{
    return g_body->GetPosition().y>20 || g_body->GetPosition().x<0;
}

void Block::change()
{
    g_body->SetLinearVelocity(b2Vec2(0,0));
    g_body->SetType(b2_staticBody);
}
