#include "Collision.h"
#include "GameObject.h"


#include "../Resource/debug.h" 

#define BLOCK_PUSH_FACTOR 0.1f
#define INF std::numeric_limits<float>::infinity()

Collision* Collision::__instance = NULL;

int CollisionEvent::WasCollided() {
    return t >= 0.0f && t <= 1.0f && obj->IsDirectionColliable(nx, ny) == 1;
}

Collision* Collision::GetInstance()
{
    if (__instance == NULL) __instance = new Collision();
    return __instance;
}

/*
    SweptAABB (Math core)
*/
void Collision::SweptAABB(
    float ml, float mt, float mr, float mb,
    float dx, float dy,
    float sl, float st, float sr, float sb,
    float& t, float& nx, float& ny)
{
    float dx_entry, dx_exit, tx_entry, tx_exit;
    float dy_entry, dy_exit, ty_entry, ty_exit;

    float t_entry;
    float t_exit;

    t = -1.0f;
    nx = ny = 0;

    // Broad-phase test 
    float bl = dx > 0 ? ml : ml + dx;
    float bt = dy > 0 ? mt : mt + dy;
    float br = dx > 0 ? mr + dx : mr;
    float bb = dy > 0 ? mb + dy : mb;

    if (br < sl || bl > sr || bb < st || bt > sb) return;
    if (dx == 0 && dy == 0) return;


    if (dx > 0) { dx_entry = sl - mr; dx_exit = sr - ml; }
    else if (dx < 0) { dx_entry = sr - ml; dx_exit = sl - mr; }
    else { dx_entry = 0.0f; dx_exit = 0.0f; }

    if (dy > 0) { dy_entry = st - mb; dy_exit = sb - mt; }
    else if (dy < 0) { dy_entry = sb - mt; dy_exit = st - mb; }
    else { dy_entry = 0.0f; dy_exit = 0.0f; }


    if (dx == 0) {
        tx_entry = -INF;
        tx_exit = INF;
    }
    else {
        tx_entry = dx_entry / dx;
        tx_exit = dx_exit / dx;
    }

    if (dy == 0) {
        ty_entry = -INF;
        ty_exit = INF;
    }
    else {
        ty_entry = dy_entry / dy;
        ty_exit = dy_exit / dy;
    }


    if (mr > sl && ml < sr && mb > st && mt < sb) {
        t = 0.0f;

        float overlap_left = mr - sl;
        float overlap_right = sr - ml;
        float overlap_top = mb - st;
        float overlap_bottom = sb - mt;

        float min_x = (overlap_left < overlap_right) ? overlap_left : overlap_right;
        float min_y = (overlap_top < overlap_bottom) ? overlap_top : overlap_bottom;

        if (min_x < min_y) {
            ny = 0.0f;
            nx = (overlap_left < overlap_right) ? -1.0f : 1.0f;
        }
        else {
            nx = 0.0f;
            ny = (overlap_top < overlap_bottom) ? -1.0f : 1.0f;
        }
        return;
    }


    if (dx != 0 && tx_entry < 0.0001f && tx_exit > 0.0f) tx_entry = 0.0f;
    if (dy != 0 && ty_entry < 0.0001f && ty_exit > 0.0f) ty_entry = 0.0f;


    if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

    t_entry = (std::max)(tx_entry, ty_entry);
    t_exit = (std::min)(tx_exit, ty_exit);

    if (t_entry > t_exit) return;

    t = t_entry;

    if (tx_entry > ty_entry) {
        ny = 0.0f;
        nx = (dx > 0) ? -1.0f : 1.0f;
    }
    else if (tx_entry < ty_entry) {
        nx = 0.0f;
        ny = (dy > 0) ? -1.0f : 1.0f;
    }
    else {
        nx = (dx > 0) ? -1.0f : 1.0f;
        ny = (dy > 0) ? -1.0f : 1.0f;
    }
}

/*
    Extension of original SweptAABB to deal with two moving objects
*/
LPCOLLISIONEVENT Collision::SweptAABB(LPGAMEOBJECT objSrc, DWORD dt, LPGAMEOBJECT objDest)
{
    float sl, st, sr, sb;       // static object bbox
    float ml, mt, mr, mb;       // moving object bbox
    float t, nx, ny;

    float mvx, mvy;
    objSrc->GetSpeed(mvx, mvy);
    float mdx = mvx * dt;
    float mdy = mvy * dt;

    float svx, svy;
    objDest->GetSpeed(svx, svy);
    float sdx = svx * dt;
    float sdy = svy * dt;


    float dx = mdx - sdx;
    float dy = mdy - sdy;

    objSrc->GetBoundingBox(ml, mt, mr, mb);
    objDest->GetBoundingBox(sl, st, sr, sb);

    SweptAABB(
        ml, mt, mr, mb,
        dx, dy,
        sl, st, sr, sb,
        t, nx, ny
    );

    CollisionEvent* e = new CollisionEvent(t, nx, ny, dx, dy, objDest, objSrc);
    return e;
}

/*
    Calculate potential collisions with the list of colliable objects
*/
void Collision::Scan(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* objDests, vector<LPCOLLISIONEVENT>& coEvents)
{
    for (UINT i = 0; i < objDests->size(); i++)
    {
        LPCOLLISIONEVENT e = SweptAABB(objSrc, dt, objDests->at(i));

        if (e->WasCollided() == 1)
            coEvents.push_back(e);
        else
            delete e;
    }
}

void Collision::Filter(LPGAMEOBJECT objSrc,
    vector<LPCOLLISIONEVENT>& coEvents,
    LPCOLLISIONEVENT& colX,
    LPCOLLISIONEVENT& colY,
    int filterBlock = 1,
    int filterX = 1,
    int filterY = 1)
{
    float min_tx, min_ty;

    min_tx = 1.0f;
    min_ty = 1.0f;
    int min_ix = -1;
    int min_iy = -1;

    for (UINT i = 0; i < coEvents.size(); i++)
    {
        LPCOLLISIONEVENT c = coEvents[i];
        if (c->isDeleted) continue;
        if (c->obj->IsDeleted()) continue;


        if (filterBlock == 1 && !c->obj->IsBlocking())
        {
            continue;
        }

        if (c->t < min_tx && c->nx != 0 && filterX == 1) {
            float sl, st, sr, sb, ml, mt, mr, mb;
            c->obj->GetBoundingBox(sl, st, sr, sb);
            objSrc->GetBoundingBox(ml, mt, mr, mb);

            if (mb <= st + 0.5f) continue;
            if (mt >= sb - 0.5f) continue;

            min_tx = c->t; min_ix = i;
        }

        if (c->t < min_ty && c->ny != 0 && filterY == 1) {
            float sl, st, sr, sb, ml, mt, mr, mb;
            c->obj->GetBoundingBox(sl, st, sr, sb);
            objSrc->GetBoundingBox(ml, mt, mr, mb);

            if (mr <= sl + 0.5f) continue;
            if (ml >= sr - 0.5f) continue;

            min_ty = c->t; min_iy = i;
        }
    }

    if (min_ix >= 0) colX = coEvents[min_ix];
    if (min_iy >= 0) colY = coEvents[min_iy];
}

/*
*  Two-Pass sliding collision framework
*/
void Collision::Process(LPGAMEOBJECT objSrc, DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    vector<LPCOLLISIONEVENT> coEvents;
    LPCOLLISIONEVENT colX = NULL;
    LPCOLLISIONEVENT colY = NULL;

    coEvents.clear();

    if (objSrc->IsCollidable())
    {
        Scan(objSrc, dt, coObjects, coEvents);
    }

    if (coEvents.size() == 0)
    {
        objSrc->OnNoCollision(dt);
    }
    else
    {
        Filter(objSrc, coEvents, colX, colY);

        float x, y, vx, vy, dx, dy;
        objSrc->GetPosition(x, y);
        objSrc->GetSpeed(vx, vy);
        dx = vx * dt;
        dy = vy * dt;

        if (colX != NULL && colY != NULL)
        {
            if (colY->t < colX->t)
            {
                y += colY->t * dy + colY->ny * BLOCK_PUSH_FACTOR;
                objSrc->SetPosition(x, y);

                objSrc->OnCollisionWith(colY);


                LPCOLLISIONEVENT colX_other = NULL;

                colX->isDeleted = true;
                coEvents.push_back(SweptAABB(objSrc, dt, colX->obj));

                Filter(objSrc, coEvents, colX_other, colY, 1, 1, 0);

                if (colX_other != NULL)
                {
                    x += colX_other->t * dx + colX_other->nx * BLOCK_PUSH_FACTOR;
                    objSrc->OnCollisionWith(colX_other);
                }
                else
                {
                    x += dx;
                }
            }
            else
            {
                x += colX->t * dx + colX->nx * BLOCK_PUSH_FACTOR;
                objSrc->SetPosition(x, y);

                objSrc->OnCollisionWith(colX);


                LPCOLLISIONEVENT colY_other = NULL;

                colY->isDeleted = true;
                coEvents.push_back(SweptAABB(objSrc, dt, colY->obj));

                Filter(objSrc, coEvents, colX, colY_other, 1, 0, 1);

                if (colY_other != NULL)
                {
                    y += colY_other->t * dy + colY_other->ny * BLOCK_PUSH_FACTOR;
                    objSrc->OnCollisionWith(colY_other);
                }
                else
                {
                    y += dy;
                }
            }
        }
        else if (colX != NULL)
        {
            x += colX->t * dx + colX->nx * BLOCK_PUSH_FACTOR;
            y += dy;
            objSrc->OnCollisionWith(colX);
        }
        else if (colY != NULL)
        {
            x += dx;
            y += colY->t * dy + colY->ny * BLOCK_PUSH_FACTOR;
            objSrc->OnCollisionWith(colY);
        }
        else
        {
            x += dx;
            y += dy;
        }

        objSrc->SetPosition(x, y);
    }


    for (UINT i = 0; i < coEvents.size(); i++)
    {
        LPCOLLISIONEVENT e = coEvents[i];
        if (e->isDeleted) continue;
        if (e->obj->IsBlocking()) continue;

        objSrc->OnCollisionWith(e);
    }

    for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}