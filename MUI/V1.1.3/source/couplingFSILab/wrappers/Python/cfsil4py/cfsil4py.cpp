#include <pybind11/pybind11.h>
#include <mpi.h>
#include <mpi4py/mpi4py.h>
#include "../../../iqnils_inl.H"

namespace py = pybind11;

PYBIND11_MODULE(cfsil4py_mod, m) {
    // optional module docstring
    m.doc() = "python bind on iqnils FSI coupling utility on MUI coupling library";

    // define Global Functions

    // bindings to class
    py::class_<muiCoupling::muiCouplingIQNILS>(m, "muiCouplingIQNILS")
        .def(py::init<double>())
         .def(py::init(
			[](int pointSize, double initUndRelxCpl, double undRelxCplMax, int aitkenIterationN, bool globalAlphaInput)
			{return new muiCoupling::muiCouplingIQNILS(pointSize, initUndRelxCpl, undRelxCplMax, aitkenIterationN, globalAlphaInput);}))
         .def(py::init(
			[](int pointSize, double initUndRelxCpl, pybind11::handle const& pyHdl, double undRelxCplMax, int aitkenIterationN, bool globalAlphaInput){
			PyObject *py_src = pyHdl.ptr();
			MPI_Comm *comm_p = PyMPIComm_Get(py_src);
			auto ric_mpiComm = reinterpret_cast<MPI_Comm>(comm_p);			
			return new muiCoupling::muiCouplingIQNILS(pointSize, initUndRelxCpl, &ric_mpiComm, undRelxCplMax, aitkenIterationN, globalAlphaInput);}))
         .def(py::init(
			[](int pointSize, double initUndRelxCpl, std::vector<int> &localRankVec, int sizeAfterSplit, double undRelxCplMax, int aitkenIterationN, bool globalAlphaInput)
			{return new muiCoupling::muiCouplingIQNILS(pointSize, initUndRelxCpl, localRankVec, sizeAfterSplit, undRelxCplMax, aitkenIterationN, globalAlphaInput);}))
        .def("undRelxCpl", &muiCoupling::muiCouplingIQNILS::undRelxCpl)
        .def("pointSize", &muiCoupling::muiCouplingIQNILS::pointSize)
        .def("residualMagSqSum", &muiCoupling::muiCouplingIQNILS::residualMagSqSum)
        .def("residualL2NormMax", &muiCoupling::muiCouplingIQNILS::residualL2NormMax)
        .def("residualL2Norm", &muiCoupling::muiCouplingIQNILS::residualL2Norm)
        .def("getXDeltaDisp", &muiCoupling::muiCouplingIQNILS::getXDeltaDisp)
        .def("getYDeltaDisp", &muiCoupling::muiCouplingIQNILS::getYDeltaDisp)
        .def("getZDeltaDisp", &muiCoupling::muiCouplingIQNILS::getZDeltaDisp)
        .def("initialize", [](muiCoupling::muiCouplingIQNILS &initialize_obj){return initialize_obj.initialize();})
        .def("initialize",
			[](muiCoupling::muiCouplingIQNILS &initialize_obj, int pointSize, double undRelxCplMax, int aitkenIterationN, bool globalAlphaInput)
			{ return initialize_obj.initialize(pointSize, undRelxCplMax, aitkenIterationN, globalAlphaInput);})
        .def("initialize",
			[](muiCoupling::muiCouplingIQNILS &initialize_obj, int pointSize, py::handle pyHdl, double undRelxCplMax, int aitkenIterationN, bool globalAlphaInput){
			PyObject *py_src = pyHdl.ptr();
			MPI_Comm *comm_p = PyMPIComm_Get(py_src);
			auto ric_mpiComm = reinterpret_cast<MPI_Comm>(comm_p);		
			return initialize_obj.initialize(pointSize, &ric_mpiComm, undRelxCplMax, aitkenIterationN, globalAlphaInput);})
		.def("collectResidual", &muiCoupling::muiCouplingIQNILS::collectResidual)
		.def("process", &muiCoupling::muiCouplingIQNILS::process);
}