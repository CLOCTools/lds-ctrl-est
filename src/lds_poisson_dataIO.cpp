#include<ldsCtrlEst>

using namespace std;
using namespace plds;

tuple<vector<armaMat>, vector<armaMat>, data_t, stdVec> plds::loadData_mat(string filename) {

	mat_t* matfile;
	matvar_t* matvar; //temp matio matvar that will be freed after loading each variable...
	const char* filename_cstr = filename.c_str();

	vector<armaMat> z;
	vector<armaMat> u;
	data_t dt;
	stdVec t0;

	size_t szZ;
	size_t szU;

	// MAT_ACC_RDWR: Read/Write access. Would be good to add the fit in form of a struct to existing mat file?
	matfile = Mat_Open(filename_cstr, MAT_ACC_RDONLY);
	if (!matfile) {
		cerr << "\nError openening file ' " << filename << " '\n";
		// TODO: define default behavior (e.g., fill with zeros?)
		return getDefaultData(z,u,dt,t0);
	}

	// dt
	matvar = Mat_VarRead(matfile, "dt");
	if (matvar) {
		data_t* dt_ptr=(data_t*)matvar->data;
		dt = dt_ptr[0];
		Mat_VarFree(matvar);
	} else {
		cerr << "\nError opening: dt \n";
		return getDefaultData(z,u,dt,t0);
		dt=-1;
	}

	// z
	matvar = Mat_VarRead(matfile, "z");
	if (matvar) {
		// loop through cells and assign to z?
		// szZ = (size_t) ((data_t) matvar->dims[0]) * ((data_t) matVar->dims[1]);
		szZ = matvar->dims[0] * matvar->dims[1];

		if (szZ) //if at least 1
		{
			matvar_t* cell = Mat_VarGetCell(matvar, 0);
			data_t* cellData = (data_t*) cell->data;
			size_t* cellDims = cell->dims;

			// this constructor for arma::Mat<T> does not inintialize memory to anything particular. I will be filling it later.
			// I don't think the should err if the different cells have different sizes. std::vector is able to reallocate.
			z = vector<armaMat>(szZ, armaMat(cellDims[0],cellDims[1],fill::zeros));
			for (size_t k=0; k<szZ; k++) {
				cell = Mat_VarGetCell(matvar, k);
				cellData = (data_t*) cell->data;
				cellDims = cell->dims;
				// This should only have to re-allocate if the dims are diff for each cell...
				z[k] = armaMat(cellData, cellDims[0], cellDims[1]);
			}
		} else {
			cerr << "\n Variable 'z' appears to be empty!\n";
			// TODO : define default behavior
			return getDefaultData(z,u,dt,t0);
		}
		Mat_VarFree(matvar);
	} else {
		cerr << "\nError opening: z\n";
		return getDefaultData(z,u,dt,t0);
	}

	// u
	matvar = Mat_VarRead(matfile, "u");
	if (matvar) {
		// make sure size of u matches size of z
		// szU = (size_t) ((data_t) matVar->dims[0]) * ((data_t) matVar->dims[1]);
		szU = matvar->dims[0] * matvar->dims[1];

		if (szU != szZ) {
			cerr << "\n ERROR: There appear to be more trials for input (u) than measurements (z). \n";
			return getDefaultData(z,u,dt,t0);
		}

		matvar_t* cell = Mat_VarGetCell(matvar, 0);
		data_t* cellData = (data_t*) cell->data;
		size_t* cellDims = cell->dims;

		// this constructor for arma::Mat<T> does not inintialize memory to anything particular. I will be filling it later.
		// I don't think the should err if the different cells have different sizes. std::vector is able to reallocate.
		u = vector<armaMat>(szU, armaMat(cellDims[0],cellDims[1]));
		for (size_t k=0; k<szU; k++) {
			cell = Mat_VarGetCell(matvar, k);
			cellData = (data_t*) cell->data;
			cellDims = cell->dims;

			// check to make sure number of time samples matches.
			if (cellDims[1] != z[k].n_cols) {
				cerr << "\n ERROR: There appear to be more time samples for input data (u) than measurements (z).\n";
				return getDefaultData(z,u,dt,t0);
			}

			u[k] = armaMat(cellData, cellDims[0], cellDims[1]);
		}
		Mat_VarFree(matvar);
	} else {
		cerr << "\nError opening: u \n";
		// TODO: Define default behavior
		return getDefaultData(z,u,dt,t0);
	}

	// t0
	matvar = Mat_VarRead(matfile, "t0");
	if (matvar) {
		size_t szT0 = matvar->dims[0] * matvar->dims[1];
		data_t* t0_ptr=(data_t*)matvar->data;
		if (szT0 != szZ) {
			if (szT0 == 1)
			t0 = stdVec(szZ, t0_ptr[0]);
			else
			t0 = stdVec(szZ, 0.0);
		} else {
			t0 = stdVec(t0_ptr,t0_ptr+szZ);
		}
		Mat_VarFree(matvar);
	} else {
		cerr << "\n Warning: Error opening t0. Assuming 0.0 ... \n";
		t0 = stdVec(szZ, 0.0);
	}

	// let's get averages for printing...
	armaVec E_n = armaVec(z[0].n_rows,fill::zeros);
	armaVec E_u = armaVec(u[0].n_rows,fill::zeros);
	data_t nSamps = 0;
	for (size_t k=0; k<szZ; k++) {
		for (size_t kk=0; kk<z[k].n_cols; kk++) {
			E_n += z[k].col(kk);
			E_u += u[k].col(kk);
			nSamps++;
		}
	}
	E_n = E_n/nSamps;
	E_u = E_u/nSamps;

	// Print out some stats for the data
	cout << "\n\nData Loaded.\n";
	cout << "No of trials: " << szZ << " \n";
	cout << "Dim. of outputs: " << z[0].n_rows << " \n";
	cout << "Dim. of inputs: " << u[0].n_rows << " \n";
	cout << "E[z]: \n" << E_n << " \n";
	cout << "E[u]: \n" << E_u << " \n\n";

	// Finally return the data
	return make_tuple(z,u,dt,t0);
}

tuple<vector<armaMat>, vector<armaMat>, data_t, stdVec> plds::getDefaultData(vector<armaMat>& z, vector<armaMat>& u, data_t& dt, stdVec& t0) {

	z = vector<armaMat>(0, armaMat(0,0,fill::zeros));
	u = vector<armaMat>(0, armaMat(0,0,fill::zeros));
	dt = -1;
	t0 = stdVec(0,0.0);

	return make_tuple(z,u,dt,t0);
}
