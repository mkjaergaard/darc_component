#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <darc/component_manager.hpp>
#include <darc/registry.hpp>
#include <darc/component.hpp>
#include <darc/component_loader.hpp>

#include <iostream>

using namespace boost::python;

BOOST_PYTHON_MODULE(darc)
{
  // Generic Stuff
//  class_<std::vector<std::string> >("vector_string")
//    .def(vector_indexing_suite<std::vector<std::string> >());

  // DARC
  class_<darc::component_manager, boost::noncopyable>("component_manager")
    .def("run", &darc::component_manager::run)
    .def("connect", &darc::component_manager::connect)
    .def("accept", &darc::component_manager::accept);

  class_<darc::Registry>("registry", no_init)
    .def("instantiate_component", &darc::Registry::instantiateComponent)
    .def("names", &darc::Registry::names)
    .def("registerComponent", &darc::Registry::registerComponent)
    .staticmethod("instantiate_component")
    .staticmethod("names")
    .staticmethod("registerComponent");

  class_<darc::component, darc::component_ptr, boost::noncopyable>("component", no_init)
    .def("run", &darc::component::run)
    .def("stop", &darc::component::stop)
    .def("pause", &darc::component::pause)
    .def("unpause", &darc::component::unpause)
    .def("name", &darc::component::get_name)
    .def("id", &darc::component::get_id, return_value_policy<copy_const_reference>())
    .staticmethod("instantiate");

  class_<darc::component_loader>("component_loader", no_init)
    .def("load_component", &darc::component_loader::load_component)
    .staticmethod("load_component");


}
