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
        vector<data_t> t0;
        data_t dt;
        size_t nX;
        size_t nH;
        armaVec d0;
        bool force_unitNormC;
        size_t whichWt;
        data_t wtG0;

        data_t t_startSSID;
        data_t t_stopSSID;

        size_t nInputs = inputs.size();
        if (nInputs<12) {
            t_stopSSID = inf;
        } else {
            t_stopSSID = (data_t) inputs[11][0];
        }

        if (nInputs<11) {
            t_startSSID = -inf;
        } else {
            t_startSSID = (data_t) inputs[10][0];
        }

        if (nInputs<10) {
          wtG0 = 0.0;
        } else {
          wtG0 = (data_t) inputs[9][0];
        }

        if (nInputs<9) {
            whichWt = 0;
        } else {
            whichWt = (size_t) inputs[8][0];
        }

        if (nInputs<8) {
            force_unitNormC = false;
        } else {
            force_unitNormC = (bool) inputs[7][0];
        }

        if (nInputs<7) {
            d0 = armaVec(1).fill(-inf);
        } else {
            TypedArray<double> d0_matlab = move(inputs[6]);
            d0 = matlabVector2armaVector<double>(d0_matlab);
        }

        if (nInputs<6) {
            nH = 50;
        } else {
            nH = (size_t) inputs[5][0];
        }

        CellArray u_matlab = move(inputs[0]);
        CellArray z_matlab = move(inputs[1]);
        TypedArray<double> t0_matlab = move(inputs[2]);

        u = matlabCell2vectorArmaMat<double>(u_matlab);
        z = matlabCell2vectorArmaMat<double>(z_matlab);
        t0 = matlabVector2vector<double>(t0_matlab);
        dt = inputs[3][0];
        nX = inputs[4][0];

        ssidFit_t pldsFit;
        lds::gaussian::ssidFit_t ldsFit;

        lds::ssidWt wt = lds::NONE;
        switch (whichWt) {
          case 0 : {
            wt = lds::NONE;
          } break;
          case 1 : {
            wt = lds::MOESP;
          } break;
          case 2 : {
            wt = lds::CVA;
          } break;
        }
        ldsFit = ssidFit(u,z,t0,dt,nX,nH,d0,force_unitNormC,wt,wtG0,t_startSSID,t_stopSSID);

        // These are two things I'm going to look at all the time, so let's go ahead and put them into the fit struct.
        data_t tau = -dt/log(ldsFit.A[0]);
        armaMat g_u2y = ldsFit.C*inv(armaMat(nX,nX,fill::eye) - ldsFit.A)*ldsFit.B + ldsFit.D; //TODO: assumes g=1

        vector<string>ldsFieldNames(15,"blank");
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
        ldsFieldNames[12]="singVals";
        ldsFieldNames[13]="tau";
        ldsFieldNames[14]="g_u2y";

        TypedArray<double> A = armaMat2matlabMat<double>(ldsFit.A, factory);
        TypedArray<double> B = armaMat2matlabMat<double>(ldsFit.B, factory);
        TypedArray<double> g = armaVec2matlabMat<double>(ldsFit.g, factory);
        TypedArray<double> m = armaVec2matlabMat<double>(ldsFit.m, factory);
        TypedArray<double> Q = armaMat2matlabMat<double>(ldsFit.Q, factory);
        TypedArray<double> x0 = armaVec2matlabMat<double>(ldsFit.x0, factory);
        TypedArray<double> P0 = armaMat2matlabMat<double>(ldsFit.P0, factory);
        TypedArray<double> C = armaMat2matlabMat<double>(ldsFit.C, factory);
        TypedArray<double> D = armaMat2matlabMat<double>(ldsFit.D, factory);
        TypedArray<double> d = armaVec2matlabMat<double>(ldsFit.d, factory);
        TypedArray<double> R = armaMat2matlabMat<double>(ldsFit.R, factory);
        TypedArray<double> singVals = armaVec2matlabMat<double>(ldsFit.singVals, factory);
        TypedArray<double> g_u2y_matlab = armaMat2matlabMat<double>(g_u2y, factory);

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
        ldsFit_matlab[0]["singVals"] = move(singVals);
        ldsFit_matlab[0]["tau"] = factory.createScalar(tau);
        ldsFit_matlab[0]["g_u2y"] = move(g_u2y_matlab);

        outputs[0] = move(ldsFit_matlab);
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
        if (outputs.size() > 1) {
            displayError("Too many outputs specified.");
        }
    }
    };
