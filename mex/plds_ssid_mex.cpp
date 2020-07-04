#include "ldsCtrlEst_mex.hpp"

using namespace std;
using namespace plds;
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
        size_t whichWt;
        data_t wtG0;
        data_t t_startSSID;
        data_t t_stopSSID;

        CellArray u_matlab = move(inputs[0]);
        CellArray z_matlab = move(inputs[1]);
        dt = inputs[2][0];
        nX = inputs[3][0];

        size_t nInputs = inputs.size();
        if (nInputs<11) {
            t_stopSSID = inf;
        } else {
            t_stopSSID = (data_t) inputs[10][0];
        }

        if (nInputs<10) {
            t_startSSID = -inf;
        } else {
            t_startSSID = (data_t) inputs[9][0];
        }

        if (nInputs<9) {
          t0 = vector<data_t>(z_matlab.getNumberOfElements(),0.0);
        } else {
          TypedArray<double> t0_matlab = move(inputs[8]);
          t0 = matlabVector2vector<double>(t0_matlab);
        }

        if (nInputs<8) {
            wtG0 = 0.0;
        } else {
            wtG0 = (data_t) inputs[7][0];
        }

        if (nInputs<7) {
            whichWt = 0;
        } else {
            whichWt = (size_t) inputs[6][0];
        }

        if (nInputs<6) {
            d0 = armaVec(1).fill(-inf);
        } else {
            TypedArray<double> d0_matlab = move(inputs[5]);
            d0 = matlabVector2armaVector<double>(d0_matlab);
        }

        if (nInputs<5) {
            nH = 50;
        } else {
            nH = (size_t) inputs[4][0];
        }

        u = matlabCell2vectorArmaMat<double>(u_matlab);
        z = matlabCell2vectorArmaMat<double>(z_matlab);

        ssidFit_t pldsFit;
        lds::gaussian::ssidFit_t gldsFit;

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
        tie(pldsFit, gldsFit) = ssidFit(u,z,dt,nX,nH,d0,wt,wtG0,t0,t_startSSID,t_stopSSID);

        // These are two things I'm going to look at all the time, so let's go ahead and put them into the fit struct.
        data_t tau = -dt/log(gldsFit.A[0]);
        armaMat g_u2y = gldsFit.C*inv(armaMat(nX,nX,fill::eye) - gldsFit.A)*gldsFit.B + gldsFit.D; //TODO: assumes g=1

        vector<string> gldsFieldNames(15,"blank");
        gldsFieldNames[0]="A";
        gldsFieldNames[1]="B";
        gldsFieldNames[2]="g";
        gldsFieldNames[3]="m";
        gldsFieldNames[4]="Q";
        gldsFieldNames[5]="x0";
        gldsFieldNames[6]="P0";
        gldsFieldNames[7]="C";
        gldsFieldNames[8]="D";
        gldsFieldNames[9]="d";
        gldsFieldNames[10]="R";
        gldsFieldNames[11]="dt";
        gldsFieldNames[12]="singVals";
        gldsFieldNames[13]="tau";
        gldsFieldNames[14]="g_u2y";

        TypedArray<double> A = armaMat2matlabMat<double>(gldsFit.A, factory);
        TypedArray<double> B = armaMat2matlabMat<double>(gldsFit.B, factory);
        TypedArray<double> g = armaVec2matlabMat<double>(gldsFit.g, factory);
        TypedArray<double> m = armaVec2matlabMat<double>(gldsFit.m, factory);
        TypedArray<double> Q = armaMat2matlabMat<double>(gldsFit.Q, factory);
        TypedArray<double> x0 = armaVec2matlabMat<double>(gldsFit.x0, factory);
        TypedArray<double> P0 = armaMat2matlabMat<double>(gldsFit.P0, factory);
        TypedArray<double> C = armaMat2matlabMat<double>(gldsFit.C, factory);
        TypedArray<double> D = armaMat2matlabMat<double>(gldsFit.D, factory);
        TypedArray<double> d = armaVec2matlabMat<double>(gldsFit.d, factory);
        TypedArray<double> R = armaMat2matlabMat<double>(gldsFit.R, factory);
        TypedArray<double> singVals = armaVec2matlabMat<double>(gldsFit.singVals, factory);
        TypedArray<double> g_u2y_matlab = armaMat2matlabMat<double>(g_u2y, factory);

        StructArray gldsFit_matlab = factory.createStructArray({1,1}, gldsFieldNames);
        gldsFit_matlab[0]["A"] = move(A);
        gldsFit_matlab[0]["B"] = move(B);
        gldsFit_matlab[0]["g"] = move(g);
        gldsFit_matlab[0]["m"] = move(m);
        gldsFit_matlab[0]["Q"] = move(Q);
        gldsFit_matlab[0]["x0"] = move(x0);
        gldsFit_matlab[0]["P0"] = move(P0);
        gldsFit_matlab[0]["C"] = move(C);
        gldsFit_matlab[0]["D"] = move(D);
        gldsFit_matlab[0]["d"] = move(d);
        gldsFit_matlab[0]["R"] = move(R);
        gldsFit_matlab[0]["dt"] = factory.createScalar(dt);
        gldsFit_matlab[0]["singVals"] = move(singVals);
        gldsFit_matlab[0]["tau"] = factory.createScalar(tau);
        gldsFit_matlab[0]["g_u2y"] = move(g_u2y_matlab);

        //Now do plds.
        vector<string> pldsFieldNames(14,"blank");
        pldsFieldNames[0]="A";
        pldsFieldNames[1]="B";
        pldsFieldNames[2]="g";
        pldsFieldNames[3]="m";
        pldsFieldNames[4]="Q";
        pldsFieldNames[5]="x0";
        pldsFieldNames[6]="P0";
        pldsFieldNames[7]="C";
        pldsFieldNames[8]="D";
        pldsFieldNames[9]="d";
        // pldsFieldNames[10]="R";
        pldsFieldNames[10]="dt";
        pldsFieldNames[11]="singVals";
        pldsFieldNames[12]="tau";
        pldsFieldNames[13]="g_u2y";

        A = armaMat2matlabMat<double>(pldsFit.A, factory);
        B = armaMat2matlabMat<double>(pldsFit.B, factory);
        g = armaVec2matlabMat<double>(pldsFit.g, factory);
        m = armaVec2matlabMat<double>(pldsFit.m, factory);
        Q = armaMat2matlabMat<double>(pldsFit.Q, factory);
        x0 = armaVec2matlabMat<double>(pldsFit.x0, factory);
        P0 = armaMat2matlabMat<double>(pldsFit.P0, factory);
        C = armaMat2matlabMat<double>(pldsFit.C, factory);
        armaMat Dzeros = gldsFit.D;
        Dzeros.fill(0.0);
        D = armaMat2matlabMat<double>(Dzeros, factory);
        d = armaVec2matlabMat<double>(pldsFit.d, factory);
        // R = armaMat2matlabMat<double>(pldsFit.R, factory);
        singVals = armaVec2matlabMat<double>(pldsFit.singVals, factory);
        g_u2y_matlab = armaMat2matlabMat<double>(g_u2y, factory);

        StructArray pldsFit_matlab = factory.createStructArray({1,1}, pldsFieldNames);
        pldsFit_matlab[0]["A"] = move(A);
        pldsFit_matlab[0]["B"] = move(B);
        pldsFit_matlab[0]["g"] = move(g);
        pldsFit_matlab[0]["m"] = move(m);
        pldsFit_matlab[0]["Q"] = move(Q);
        pldsFit_matlab[0]["x0"] = move(x0);
        pldsFit_matlab[0]["P0"] = move(P0);
        pldsFit_matlab[0]["C"] = move(C);
        pldsFit_matlab[0]["D"] = move(D);
        pldsFit_matlab[0]["d"] = move(d);
        // pldsFit_matlab[0]["R"] = move(R);
        pldsFit_matlab[0]["dt"] = factory.createScalar(dt);
        pldsFit_matlab[0]["singVals"] = move(singVals);
        pldsFit_matlab[0]["tau"] = factory.createScalar(tau);
        pldsFit_matlab[0]["g_u2y"] = move(g_u2y_matlab);

        outputs[0] = move(pldsFit_matlab);
        outputs[1] = move(gldsFit_matlab);
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
        if (outputs.size() > 2) {
            displayError("Too many outputs specified.");
        }
    }
    };
