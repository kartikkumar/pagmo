# -*- coding: iso-8859-1 -*-
# Copyright (C) 2004-2009 The PaGMO development team,
# Advanced Concepts Team (ACT), European Space Agency (ESA)
# http://apps.sourceforge.net/mediawiki/pagmo
# http://apps.sourceforge.net/mediawiki/pagmo/index.php?title=Developers
# http://apps.sourceforge.net/mediawiki/pagmo/index.php?title=Credits
# act@esa.int
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the
# Free Software Foundation, Inc.,
# 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
  
## @package robot
#  This module contains the ALife Robot class.
#
#  @author John Glover
from pybrain.rl.environments.ode import sensors, actuators
import ode
import xml.dom.minidom as md
import numpy as np
        
        
## Robot class
#
#  Defines a robot that will be used in the ALife simulation.
#  For now this is just a body (box) with cylindrical legs. 
#  Each leg is attached to the body with a hinge joint.
#
#  The constructor takes a optional 3D position vector, which 
#  the body will be centered on.
class Robot(object):
    ## Constructor
    #  @param world The ODE world that bodies are added to
    #  @param space The ODE space that geometries are added to
    #  @param body_position A 3-tuple giving the initial position of the
    #                       robot body
    #  @param name The string containing the name of the robot
    def __init__(self, world, space, body_position=[0.0, 150.0, 0.0], name=""):
        ##
        self.world = world
        ## 
        self.space = space
        ##
        self.bodies_geoms = []
        ##
        self.joints = []
        ##
        self.name = name
        ##
        self._body_sections = 1
        ## The density of the body
        self._body_density = 0.35
        # the size of the body
        self._body_size = [4.0, 3.0, 4.0]
        ##
        self._legs = 4
        # radius of the legs
        self._leg_radius = 0.25
        # length of the legs
        self._leg_length = 3.8
        # density of the legs
        self._leg_density = 0.25
        ## Offset used to calculate leg y-axis coordinate.
        #  The last term makes the legs recede into the body slightly, looks
        #  a bit better
        self._leg_y_offset = ((self._leg_length/2) + 
                              (self._body_size[1]/2) - 
                              min(self._leg_radius*2, 1.0))
        # The rotation of the legs
        self._leg_rotation = (1.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0)
        ##
        self.passpairs = []
        ##
        self.center_obj = "robot_body"
        
        ## Main body section. There must always be at least 1 body section.
        self.main_body = ode.Body(world)
        self.main_body.name = "robot_body"
        self.main_body.setPosition(body_position)
        self.main_body.initial_position = body_position
        body_mass = ode.Mass()
        body_mass.setBox(self._body_density, self._body_size[0], 
                         self._body_size[1], self._body_size[2])
        self.main_body.setMass(body_mass)
        ##
        self.main_geom = ode.GeomBox(space, lengths=self._body_size)
        self.main_geom.name = "robot_body"
        self.main_geom.setBody(self.main_body)
        self.main_body.initial_rotation = self.main_body.getRotation()
        self.bodies_geoms.append((self.main_body, self.main_geom))
        # create legs for main body section
        self._create_legs(self.main_body)
        # create additional body sections and legs
        if self._body_sections > 1:
            self._create_body_sections()
        
    def _create_body_sections(self):
        # todo: add the ability to create robots with multiple body sections.
        # These could be connected by simple joints, creating a robot that could
        # try to move like a snake rather than by walking
        pass
    
    def _create_legs(self, body):
        for i in range(self._legs):
            self._create_leg(body, i)
        
    def _create_leg(self, body, n):
        leg = ode.Body(self.world)
        leg.name = "robot_leg" + str(n+1)
        # calculate leg spacing based on the size of the body and the 
        # number of legs on each side of the z-axis
        legs_per_side = (self._legs / 2)
        leg_z_spacing = (self._body_size[2]/2.0) / 2.0
        # calculate leg x-axis offset        
        if legs_per_side % 2 != 0:
            leg_x_spacing = self._body_size[0] / legs_per_side
            if n < 2:
                leg_x_offset = 0
            else:
                if n % 4 > 1:
                    leg_x_offset = (((n-2)/4) + 1) * leg_x_spacing
                else:
                    leg_x_offset = (((n-2)/4) + 1) * -leg_x_spacing
        else:
            leg_x_spacing = (self._body_size[0]/2.0) / legs_per_side
            if n % 4 > 1:
                leg_x_offset = ((n/4) + 1) * leg_x_spacing
            else:
                leg_x_offset = ((n/4) + 1) * -leg_x_spacing
        # calculate leg z-axis offset
        if n % 2 == 0:
            leg_z_offset = leg_z_spacing
        else:
            leg_z_offset = -leg_z_spacing
        # calculate absolute position for the leg using the body
        # position and the leg offset
        body_position = body.getPosition()
        leg_position = (body_position[0]+leg_x_offset, 
                        body_position[1]-self._leg_y_offset, 
                        body_position[2]+leg_z_offset)
        leg.setPosition(leg_position)
        leg.initial_position = leg_position
        leg_mass = ode.Mass()
        leg_mass.setCappedCylinder(self._leg_density, 3, 
                                   self._leg_radius, self._leg_length)
        leg.setMass(leg_mass)
        leg_geom = ode.GeomCCylinder(self.space, length=self._leg_length, 
                                     radius=self._leg_radius)
        leg_geom.name = leg.name
        leg_geom.setBody(leg)
        leg.setRotation(self._leg_rotation)
        leg.initial_rotation = self._leg_rotation
        # add to the list of bodies and geometries
        self.bodies_geoms.append((leg, leg_geom))
        # set it so that there is no collision detection between this leg
        # and the body that it is attached to, otherwise the simulation can
        # become unstable.
        self.passpairs.append((body.name, leg.name))
        # create a hinge joint connecting each leg to the body
        self._create_leg_joint(body, leg, n)
        
    def _create_leg_joint(self, body, leg, n):
        body_position = body.getPosition()
        leg_position = leg.getPosition()
        joint = ode.HingeJoint(self.world)
        joint.name= body.name + "_leg" + str(n+1)
        joint.attach(body, leg)
        joint.setAnchor((leg_position[0], 
                         body_position[1]-(self._body_size[1]/2.0), 
                         leg_position[2]))
        joint.setAxis((1,0,0))
        joint.setParam(ode.ParamLoStop, -1.2)
        joint.setParam(ode.ParamHiStop, 1.2)
        joint.setParam(ode.ParamFMax, 10)
        self.joints.append(joint)
                          
    def get_position(self):
        return self.main_body.getPosition()    
        
    def get_joints(self):
        return self.joints
    
    def get_num_legs(self):
        return self._legs
    
    def set_num_legs(self, n):
        if n % 2 != 0:
            # todo: more detail on this exception
            raise Exception("OddLegNumber")
        self._legs = n
        self.bodies_geoms = []
        self.bodies_geoms.append((self.main_body, self.main_geom))
        self._create_legs(self.main_body)
        
    def get_body_density(self):
        return self._body_density
    
    def set_body_density(self, density):
        self._body_density = density
        body_mass = self.main_body.getMass()
        body_mass.setBox(self._body_density, self._body_size[0], 
                         self._body_size[1], self._body_size[2])
    
    def get_leg_density(self):
        return self._leg_density
    
    def set_leg_density(self, density):
        self._leg_density = density
        for body, geom in self.bodies_geoms:
            if body.name[0:len("robot_leg")] == "robot_leg":
                leg_mass = body.getMass()
                leg_mass.setCappedCylinder(self._leg_density, 3, 
                                           self._leg_radius, self._leg_length)

    def reset(self):
        for body, geom in self.bodies_geoms:
            body.setPosition(body.initial_position)
            body.setRotation(body.initial_rotation)
            body.setLinearVel((0, 0, 0))
            body.setAngularVel((0, 0, 0))
        for joint in self.joints:
            joint.setParam(ode.ParamVel, 0)
