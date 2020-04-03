/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2019 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU General Public License as published by the Free  *
* Software Foundation; either version 2 of the License, or (at your option)   *
* any later version.                                                          *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for    *
* more details.                                                               *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program. If not, see <http://www.gnu.org/licenses/>.              *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#include <sofa/helper/ArgumentParser.h>
#include <SofaSimulationGraph/init.h>
#include <SofaSimulationGraph/DAGSimulation.h>
#include <sofa/simulation/Node.h>
#include <SofaGraphComponent/Gravity.h>
#include <SofaExplicitOdeSolver/EulerSolver.h>
#include <SofaBaseVisual/VisualStyle.h>
#include <sofa/core/objectmodel/Context.h>
#include <SofaBaseCollision/SphereModel.h>
#include <sofa/core/VecId.h>
#include <sofa/gui/GUIManager.h>
#include <sofa/gui/Main.h>

#include <sofa/helper/system/glut.h>
#include <sofa/helper/accessor.h>

#include <SofaCommon/initCommon.h>
#include <SofaBase/initBase.h>
#include <SofaGeneral/initGeneral.h>
#include <SofaAdvanced/initAdvanced.h>
#include <SofaMisc/initMisc.h>



using sofa::component::odesolver::EulerSolver;
using namespace sofa::component::collision;
using sofa::core::objectmodel::Data;
using sofa::helper::ReadAccessor;
using sofa::helper::WriteAccessor;
using sofa::core::VecId;
using sofa::core::objectmodel::New;

// ---------------------------------------------------------------------
// ---
// ---------------------------------------------------------------------
int main(int argc, char** argv)
{
    glutInit(&argc,argv);
    sofa::simulation::graph::init();
    sofa::component::initBase();
    sofa::component::initCommon();
    sofa::component::initGeneral();
    sofa::component::initAdvanced();
    sofa::component::initMisc();
    sofa::gui::initMain();

    sofa::helper::parse("This is a SOFA application.")
    (argc,argv);

    // The graph root node
    sofa::simulation::setSimulation(new sofa::simulation::graph::DAGSimulation());
    sofa::simulation::Node::SPtr groot = sofa::simulation::getSimulation()->createNewGraph("root");
    groot->setGravity( Coord3(0,-10,0) );

    // One solver for all the graph
    EulerSolver::SPtr solver = sofa::core::objectmodel::New<EulerSolver>();
    solver->setName("solver");
    solver->f_printLog.setValue(false);
    groot->addObject(solver);

    // One node to define the particle
    sofa::simulation::Node::SPtr particule_node = groot.get()->createChild("particle_node");
    // The particule, i.e, its degrees of freedom : a point with a velocity
    MechanicalObject3::SPtr dof = sofa::core::objectmodel::New<MechanicalObject3>();
    dof->setName("particle");
    particule_node->addObject(dof);
    dof->resize(1);
    // get write access the particle positions vector
    WriteAccessor< Data<MechanicalObject3::VecCoord> > positions = *dof->write( VecId::position() );
    positions[0] = Coord3(0,0,0);
    // get write access the particle velocities vector
    WriteAccessor< Data<MechanicalObject3::VecDeriv> > velocities = *dof->write( VecId::velocity() );
    velocities[0] = Deriv3(0,0,0);
    // show the particle
    dof->showObject.setValue(true);
    dof->showObjectScale.setValue(10.);

    // Its properties, i.e, a simple mass node
    UniformMass3::SPtr mass = sofa::core::objectmodel::New<UniformMass3>();
    mass->setName("mass");
    particule_node->addObject(mass);
    mass->setMass( 1 );

    // this currently reveals a bug
//    // attach a collision surface to the particle
//    SphereCollisionModel<sofa::defaulttype::Vec3Types>::SPtr sphere = New<SphereCollisionModel<sofa::defaulttype::Vec3Types>>();
//    particule_node->addObject(sphere);
//    sphere->defaultRadius.setValue(0.1);

    // Display Flags
    sofa::component::visualmodel::VisualStyle::SPtr style = sofa::core::objectmodel::New<sofa::component::visualmodel::VisualStyle>();
    groot->addObject(style);
    sofa::core::visual::DisplayFlags& flags = *style->displayFlags.beginEdit();
    flags.setShowBehaviorModels(true);
    flags.setShowCollisionModels(true);
    style->displayFlags.endEdit();

    sofa::simulation::graph::getSimulation()->init(groot.get());
    groot->setAnimate(false);

    //======================================
    // Set up the GUI
    sofa::gui::GUIManager::Init(argv[0]);
    sofa::gui::GUIManager::createGUI(groot);
    sofa::gui::GUIManager::SetDimension(800,700);
//    sofa::gui::GUIManager::SetFullScreen();  // why does this not work ?


    //=======================================
    // Run the main loop
    sofa::gui::GUIManager::MainLoop(groot);

    sofa::simulation::graph::cleanup();
    return 0;
}
