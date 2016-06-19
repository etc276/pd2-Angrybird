#include "bird.h"

Bird::Bird(float x, float y, float radius, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene):GameItem(world)
{
    // Set pixmap
    g_pixmap.setPixmap(pixmap);
    g_pixmap.setTransformOriginPoint(g_pixmap.boundingRect().width()/2,g_pixmap.boundingRect().height()/2);
    g_size = QSize(radius*2,radius*2);

    // Create Body
    b2BodyDef bodydef;
    bodydef.type = b2_dynamicBody;
    bodydef.bullet = true;
    bodydef.position.Set(x,y);
    bodydef.userData = this;
    g_body = world->CreateBody(&bodydef);
    b2CircleShape bodyshape;
    bodyshape.m_radius = radius;
    b2FixtureDef fixturedef;
    fixturedef.shape = &bodyshape;
    fixturedef.density = BIRD_DENSITY;
    fixturedef.friction = BIRD_FRICTION;
    fixturedef.restitution = BIRD_RESTITUTION;
    g_body->SetAngularDamping(3);
    g_body->CreateFixture(&fixturedef);

    // Bound timer
    connect(timer, SIGNAL(timeout()), this,SLOT(paint()));

    scene->addItem(&g_pixmap);
}

void Bird::setLinearVelocity(b2Vec2 velocity)
{
    g_body->SetLinearVelocity(velocity);
}

void Bird::skill_red ()
{

}

void Bird::skill_yellow ()
{
    b2Vec2 velocity = g_body->GetLinearVelocity();
    velocity.operator *=(4);
    g_body->SetLinearVelocity(velocity);
}

void Bird::skill_green ()
{
    g_body->SetLinearVelocity(b2Vec2(-20,-10));
}

void Bird::skill_blue ()
{
    g_body->SetLinearVelocity(b2Vec2(0,-30));
}

int Bird::getScore()
{
    double Vx = g_body->GetLinearVelocity().x;
    double Vy = g_body->GetLinearVelocity().y;
    return (int)sqrt(Vx*Vx + Vy*Vy);
}
