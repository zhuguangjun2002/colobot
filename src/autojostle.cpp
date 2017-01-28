// autojostle.cpp

#define STRICT
#define D3D_OVERLOADS

#include <windows.h>
#include <stdio.h>
#include <d3d.h>

#include "struct.h"
#include "D3DEngine.h"
#include "D3DMath.h"
#include "event.h"
#include "misc.h"
#include "iman.h"
#include "math3d.h"
#include "particule.h"
#include "light.h"
#include "terrain.h"
#include "camera.h"
#include "object.h"
#include "interface.h"
#include "button.h"
#include "list.h"
#include "window.h"
#include "sound.h"
#include "auto.h"
#include "autojostle.h"




// Constructeur de l'objet.

CAutoJostle::CAutoJostle(CInstanceManager* iMan, CObject* object)
						   : CAuto(iMan, object)
{
	CAuto::CAuto(iMan, object);

	Init();
}

// Destructeur de l'objet.

CAutoJostle::~CAutoJostle()
{
	CAuto::~CAuto();
}


// D�truit l'objet.

void CAutoJostle::DeleteObject(BOOL bAll)
{
	CAuto::DeleteObject(bAll);
}


// Initialise l'objet.

void CAutoJostle::Init()
{
	m_time = 0.0f;
	m_error = ERR_CONTINUE;
	m_zoom = m_object->RetZoom(0);

	CAuto::Init();
}


// D�marre une �mission.

void CAutoJostle::Start(int param, float force)
{
	ObjectType	type;

	if ( force < 0.0f )  force = 0.0f;
	if ( force > 1.0f )  force = 1.0f;

	m_force = force;
	m_progress = 0.0f;
	m_speed = 1.0f/(0.5f+force*1.0f);  // 0.5 .. 1.5
	m_time = 0.0f;
	m_error = ERR_CONTINUE;

	type = m_object->RetType();
	if ( type >= OBJECT_PLANT5 &&
		 type <= OBJECT_PLANT7 )  // tr�fle ?
	{
		m_force *= 3.0f;
	}
}


// Gestion d'un �v�nement.

BOOL CAutoJostle::EventProcess(const Event &event)
{
	D3DVECTOR	dir;
	float		factor, angle, zoom;

	CAuto::EventProcess(event);

	if ( m_engine->RetPause() )  return TRUE;
	if ( event.event != EVENT_FRAME )  return TRUE;

	if ( m_progress < 1.0f )
	{
		m_progress += event.rTime*m_speed;

		if ( m_progress < 0.5f )
		{
			factor = m_progress/0.5f;
		}
		else
		{
			factor = 2.0f-m_progress/0.5f;
		}
		factor *= m_force;

		dir.x = sinf(m_progress*PI*4.0f);
		dir.z = cosf(m_progress*PI*4.0f);

		angle = sinf(m_time*10.0f)*factor*0.04f;
		m_object->SetAngleX(0, angle*dir.z);
		m_object->SetAngleZ(0, angle*dir.x);

		zoom = 1.0f+sinf(m_time*8.0f)*factor*0.06f;
		m_object->SetZoomX(0, zoom*m_zoom.x);
		zoom = 1.0f+sinf(m_time*5.0f)*factor*0.06f;
		m_object->SetZoomY(0, zoom*m_zoom.y);
		zoom = 1.0f+sinf(m_time*7.0f)*factor*0.06f;
		m_object->SetZoomZ(0, zoom*m_zoom.z);
	}
	else
	{
		m_object->SetAngleX(0, 0.0f);
		m_object->SetAngleZ(0, 0.0f);
		m_object->SetZoom(0, m_zoom);
		m_error = ERR_STOP;
	}

	return TRUE;
}


// Indique si l'automate a termin� son activit�.

Error CAutoJostle::IsEnded()
{
	return m_error;
}

