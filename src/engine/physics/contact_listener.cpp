/*************************************************************************/
/*  contact_listener.cpp                                                 */
/*************************************************************************/
/*                       This file is part of:                           */
/*                            SUN-0 Engine                               */
/*                            sinz.com.br                                */
/*************************************************************************/
/* Copyright (c) 2018-2020 Sinz Group.                                   */
/*                                                                       */
/* This program is free software: you can redistribute it and/or modify  */
/* it under the terms of the GNU General Public License as published by  */
/* the Free Software Foundation, either version 3 of the License, or     */
/* (at your option) any later version.                                   */
/*                                                                       */
/* This program is distributed in the hope that it will be useful,       */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of        */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         */
/* GNU General Public License for more details.                          */
/*                                                                       */
/* You should have received a copy of the GNU General Public License     */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>. */
/*                                                                       */
/*************************************************************************/
#include "contact_listener.hpp"

namespace sun {

void ContactListener::BeginContact(b2Contact* contact)
{
    //void* bodyA_user_data = contact->GetFixtureA()->GetBody()->GetUserData();
	//void* bodyB_user_data = contact->GetFixtureB()->GetBody()->GetUserData();

    //if (bodyA_user_data == nullptr || bodyB_user_data == nullptr) {
    //    return;
    //}

    //RigidBodyComponent* body_a = static_cast<RigidBodyComponent*>(bodyA_user_data);
    //RigidBodyComponent* body_b = static_cast<RigidBodyComponent*>(bodyB_user_data);

    // handle collision suspension

    /*if (body_a->_collision_suspension._flag && body_b->_parent->get_id() == body_a->_collision_suspension._entity_id)
    {
        body_a->_collision_suspension._flag = false;
        contact->SetEnabled(false);
    }
    if(body_b->_collision_suspension._flag && body_a->_parent->get_id() == body_b->_collision_suspension._entity_id)
    {
        body_b->_collision_suspension._flag = false;
        contact->SetEnabled(false);
    }*/

    // one-way collision

    //b2Body* common_b2body = nullptr;
    //bool we_are_dealing_with_one_way_collision = false;

    /*if (body_a->_is_one_way_collision && body_b->_body_type == RigidBodyComponent::_b2_body_type_::DYNAMIC)
    {
        common_b2body = contact->GetFixtureB()->GetBody();
        we_are_dealing_with_one_way_collision = true;
    }
    else if (body_b->_is_one_way_collision && body_a->_body_type == RigidBodyComponent::_b2_body_type_::DYNAMIC)
    {
        common_b2body = contact->GetFixtureA()->GetBody();
        we_are_dealing_with_one_way_collision = true;
    }*/

    //if (we_are_dealing_with_one_way_collision)
    //{
    //    bool should_be_solid = false;
    //    int n_points = contact->GetManifold()->pointCount;
    //    b2WorldManifold manifold;
    //    contact->GetWorldManifold(&manifold);

        // check if contact points are moving downwards
    //    for (int i=0 ; i < n_points ; ++i)
    //    {
    //        b2Vec2 point_vel_common = common_b2body->
    //                GetLinearVelocityFromWorldPoint(manifold.points[i]);

    //        if (point_vel_common.y < 0.5f)
    //        {
    //            should_be_solid = true;
    //            break;
    //        }
    //    }

        // if any point is moving into a one way body,
        // leave contact solid.
    //    if(!should_be_solid)
    //        contact->SetEnabled(false); // make it not solid otherwise
    //}

    // fire callback

    //if(body_a->_collision_callback)
      //  body_a->_parent->on_collision(*body_b->_parent);
    //if(body_b->_collision_callback)
      //  body_b->_parent->on_collision(*body_a->_parent);
}

void ContactListener::EndContact(b2Contact* contact)
{
    /*contact->SetEnabled(true);

	void* bodyA_user_data = contact->GetFixtureA()->GetBody()->GetUserData();
	void* bodyB_user_data = contact->GetFixtureB()->GetBody()->GetUserData();

	if (bodyA_user_data == nullptr || bodyB_user_data == nullptr) {
	    return;
	}*/

    //RigidBodyComponent* body_a = static_cast<RigidBodyComponent*>(bodyA_user_data);
    //RigidBodyComponent* body_b = static_cast<RigidBodyComponent*>(bodyB_user_data);

    /*if(body_a->_collision_callback)
        body_a->_parent->on_collision_end(*body_b->_parent);
    if(body_b->_collision_callback)
        body_b->_parent->on_collision_end(*body_a->_parent);*/
}

}
