#include "ldsCtrlEst_mex.hpp"

using namespace std;
using namespace glds;
using namespace matlab::data;
using matlab::mex::ArgumentList;

class MexFunction : public matlab::mex::Function {
private:
    std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr;

public:
    /* Constructor for the class. */
    MexFunction()
    {
        matlabPtr = getEngine();
    }

    void operator()(ArgumentList outputs, ArgumentList inputs) {
        checkArguments(outputs, inputs);
        matlab::data::ArrayFactory factory;

        // Input Parameters
        vector<armaMat> u;
        vector<armaMat> z;
        // vector<data_t> t0;
        data_t dt;
        bool calcAB = true;
        bool calcQ = true;
        bool calcInitial = true;
        bool calcC = true;
        bool calcd = true;
        bool calcR = true;
        size_t maxIter = 100;
        data_t tol = 1e-3;

        // original fit.
        matlab::data::StructArray fit0 = move(inputs[0]);
        CellArray u_matlab = move(inputs[1]);
        CellArray z_matlab = move(inputs[2]);
        dt = inputs[3][0];

        u = matlabCell2vectorArmaMat<double>(u_matlab);
        z = matlabCell2vectorArmaMat<double>(z_matlab);
        // t0 = vector<data_t>(z_matlab.getNumberOfElements(),0.0);

        if (inputs.size()>4) {calcAB = (bool) inputs[4][0];};
        if (inputs.size()>5) {calcQ = (bool) inputs[5][0];};
        if (inputs.size()>6) {calcInitial = (bool) inputs[6][0];};
        if (inputs.size()>7) {calcC = (bool) inputs[7][0];};
        if (inputs.size()>8) {calcd = (bool) inputs[8][0];};
        if (inputs.size()>9) {calcR = (bool) inputs[9][0];};
        if (inputs.size()>10) {maxIter = (size_t) inputs[10][0];};
        if (inputs.size()>11) {tol = (data_t) inputs[11][0];};

        // get the model parameters.
        armaMat A_0 = matlabMat2armaMat<double>(fit0[0]["A"]);
        armaMat B_0 = matlabMat2armaMat<double>(fit0[0]["B"]);
        armaVec g_0 = matlabVector2armaVector<double>(fit0[0]["g"]);
        armaVec m_0 = matlabVector2armaVector<double>(fit0[0]["m"]);
        armaMat Q_0 = matlabMat2armaMat<double>(fit0[0]["Q"]);
        armaVec x0_0 = matlabVector2armaVector<double>(fit0[0]["x0"]);
        armaMat P0_0 = matlabMat2armaMat<double>(fit0[0]["P0"]);

        armaMat C_0 = matlabMat2armaMat<double>(fit0[0]["C"]);
        armaVec d_0 = matlabVector2armaVector<double>(fit0[0]["d"]);
        armaMat R_0 = matlabMat2armaMat<double>(fit0[0]["R"]);

        emFit_t ldsFit(A_0, B_0, g_0, m_0, Q_0, x0_0, P0_0, C_0, d_0, R_0, dt, u, z);
        ldsFit.maxIter = maxIter;
        ldsFit.tol = tol;

        ldsFit.runEM(calcAB, calcQ, calcInitial, calcC, calcd, calcR);

        vector<string>ldsFieldNames(12,"blank");
        ldsFieldNames[0]="A";
        ldsFieldNames[1]="B";
        ldsFieldNames[2]="g";
        ldsFieldNames[3]="m";
        ldsFieldNames[4]="Q";
        ldsFieldNames[5]="x0";
        ldsFieldNames[6]="P0";
        ldsFieldNames[7]="C";
        ldsFieldNames[8]="D";
        ldsFieldNames[9]="d";
        ldsFieldNames[10]="R";
        ldsFieldNames[11]="dt";

        armaMat D0(ldsFit.C.n_rows, ldsFit.B.n_cols, arma::fill::zeros);

        TypedArray<double> A = armaMat2matlabMat<double>(ldsFit.A, factory);
        TypedArray<double> B = armaMat2matlabMat<double>(ldsFit.B, factory);
        TypedArray<double> g = armaVec2matlabMat<double>(ldsFit.g, factory);
        TypedArray<double> m = armaVec2matlabMat<double>(ldsFit.m, factory);
        TypedArray<double> Q = armaMat2matlabMat<double>(ldsFit.Q, factory);
        TypedArray<double> x0 = armaVec2matlabMat<double>(ldsFit.x0, factory);
        TypedArray<double> P0 = armaMat2matlabMat<double>(ldsFit.P0, factory);
        TypedArray<double> C = armaMat2matlabMat<double>(ldsFit.C, factory);
        TypedArray<double> D = armaMat2matlabMat<double>(D0, factory);
        TypedArray<double> d = armaVec2matlabMat<double>(ldsFit.d, factory);
        TypedArray<double> R = armaMat2matlabMat<double>(ldsFit.R, factory);

        StructArray ldsFit_matlab = factory.createStructArray({1,1}, ldsFieldNames);
        ldsFit_matlab[0]["A"] = move(A);
        ldsFit_matlab[0]["B"] = move(B);
        ldsFit_matlab[0]["g"] = move(g);
        ldsFit_matlab[0]["m"] = move(m);
        ldsFit_matlab[0]["Q"] = move(Q);
        ldsFit_matlab[0]["x0"] = move(x0);
        ldsFit_matlab[0]["P0"] = move(P0);
        ldsFit_matlab[0]["C"] = move(C);
        ldsFit_matlab[0]["D"] = move(D);
        ldsFit_matlab[0]["d"] = move(d);
        ldsFit_matlab[0]["R"] = move(R);
        ldsFit_matlab[0]["dt"] = factory.createScalar(dt);

        outputs[0] = move(ldsFit_matlab);
        if (outputs.size()>1)
        {
            vector<armaMat> yHat = ldsFit.getY();
            size_t nTrials = yHat.size();
            CellArray yHat_m = factory.createCellArray({nTrials,1});
            for (size_t trial=0; trial<nTrials; trial++)
            {
                yHat_m[trial] = move(armaMat2matlabMat<double>(yHat[trial], factory));
            }
            outputs[1] = move(yHat_m);
        }

        if (outputs.size()>2)
        {
            vector<armaMat> xHat = ldsFit.getX();
            size_t nTrials = xHat.size();
            CellArray xHat_m = factory.createCellArray({nTrials,1});
            for (size_t trial=0; trial<nTrials; trial++)
            {
                xHat_m[trial] = move(armaMat2matlabMat<double>(xHat[trial], factory));
            }
            outputs[2] = move(xHat_m);
        }

        if (outputs.size()>3)
        {
            armaMat sum_E_xu_tm1_xu_tm1 = ldsFit.get_sum_E_xu_tm1_xu_tm1();
            TypedArray<double> sum_E_xu_tm1_xu_tm1_m = armaMat2matlabMat<double>(sum_E_xu_tm1_xu_tm1, factory);
            outputs[3] = move(sum_E_xu_tm1_xu_tm1_m);
        }
        if (outputs.size()>4)
        {
            armaMat sum_E_xu_t_xu_tm1 = ldsFit.get_sum_E_xu_t_xu_tm1();
            TypedArray<double> sum_E_xu_t_xu_tm1_m = armaMat2matlabMat<double>(sum_E_xu_t_xu_tm1, factory);
            outputs[4] = move(sum_E_xu_t_xu_tm1_m);
        }
        if (outputs.size()>5)
        {
            armaMat sum_E_xu_t_xu_t = ldsFit.get_sum_E_xu_t_xu_t();
            TypedArray<double> sum_E_xu_t_xu_t_m = armaMat2matlabMat<double>(sum_E_xu_t_xu_t, factory);
            outputs[5] = move(sum_E_xu_t_xu_t_m);
        }
        if (outputs.size()>6)
        {
            size_t T = ldsFit.getT();
            TypedArray<double> T_m = factory.createScalar<double>(T);
            outputs[6] = move(T_m);
        }
    }

    /* Helper function to print output string on MATLAB command prompt. */
    // void displayOnMATLAB(std::ostringstream stream)
    // {
    //   ArrayFactory factory;
    //   matlabPtr->feval(u"fprintf", 0, std::vector<Array>
    //           ({ factory.createScalar(stream.str())}));
    // }

    /* Helper function to generate an error message from given string,
    * and display it over MATLAB command prompt.
    */
    void displayError(std::string errorMessage)
    {
        ArrayFactory factory;
        matlabPtr->feval( u"error", 0, std::vector<Array>({
            factory.createScalar(errorMessage)}) );
    }

    void checkArguments(ArgumentList outputs, ArgumentList inputs)
    {
        if (inputs.size() < 5) {
            displayError("At least 5 arguments required.");
        }
        if (outputs.size() > 7) {
            displayError("Too many outputs requested.");
        }
    }
    };
