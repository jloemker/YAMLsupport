
#include "TFile.h"
#include "TH1D.h"

#include <iostream>
#include <fstream>

TH1D *make_ratio(TH1D* h1, TH1D* h2){
    TH1D *ratio;
    ratio = (TH1D*) h1->Clone("ratio");
    ratio->Divide(h2);
    return ratio;
}

void create(TString var, TString out, TH1D* data, TH1D* err, TString cutValues, TString cutName, TString system, TString centr){

    ofstream hep(out);
    // -------------------------------
    // helper to write one value block
    // -------------------------------
    auto write_R_block = [&](const char* rpair,
                            TH1D* h,
                            TH1D* hErr,
                            TString varCut,
                            TString system,
                            TString centr)
    {
        hep << "- header: {name: "<<system<<", units: "<<centr<<"}\n";
        hep << "  qualifiers:\n";
        hep << "  - {name: R_jet, value: 0.2} \n";
        hep << "  - {name: Jet_algorithm, value: Anti-k_T} \n";
        hep << "  - {name: z_SD, value: 0.2} \n";
        hep << "  - {name: beta_SD, value: 0.0} \n";
        hep << "  - {name: p_Tjet, value: 60-80 GeV/c} \n";
        hep << "  - {name: eta_jet, value: -0.7 < eta_jet < 0.7 } \n";
        hep << "  - {name: "<<varCut<<", value: "<< rpair <<"}\n";

        hep << "  values:\n";

        int nbins = h->GetNbinsX();

        for (int i = 1; i <= nbins-1; i++) {

            double val = h->GetBinContent(i+1);
            double stat = h->GetBinError(i+1);
            double sys = hErr->GetBinError(i+1);
            hep << "  - value: " << val << "\n";
            hep << "    errors:\n";
            hep << "    - {label: stat, symerror: " << stat << "}\n";
            hep << "    - {label: sys,  symerror: " << sys  << "}\n";
        }
    };
    
    // -------------------------------
    // independent variable (Rg bins)
    // -------------------------------
    hep << "independent_variables:\n";
    hep << "- header:\n";
    hep << "    name: "<<var<<"\n";
    //hep << "    units: GeV/c\n";
    hep << "  values:\n";

    int nbins = data->GetNbinsX();

    for (int i = 1; i <= nbins-1; i++) {
        hep << "  - low: " << data->GetXaxis()->GetBinLowEdge(i+1) << "\n";
        hep << "    high: " << data->GetXaxis()->GetBinUpEdge(i+1) << "\n";
    }

    // -----------------------------------
    // dependent variables (Zg/Rg ranges)
    // -----------------------------------
    hep << "\ndependent_variables:\n";

    write_R_block(cutValues, data, err, cutName, system, centr);

    hep.close();

    cout << "✔ HEPData file written: "<< out << endl;
}

void createYAML(){
    //Files for PbPb
    TFile *fileAA = new TFile("AA/Projections.root", "READ");
    //Rg
    TH1D *aa_hProj_Rg_lowZg = (TH1D*) fileAA->Get("aa_hProj_Rg_lowZg");
    TH1D *aa_hError_Rg_lowZg = (TH1D*) fileAA->Get("aa_hError_Rg_lowZg");

    TH1D *aa_hProj_Rg_highZg = (TH1D*) fileAA->Get("aa_hProj_Rg_highZg");
    TH1D *aa_hError_Rg_highZg = (TH1D*) fileAA->Get("aa_hError_Rg_highZg");

    //Zg
    TH1D *aa_hProj_Zg_lowRg = (TH1D*) fileAA->Get("aa_hProj_Zg_lowRg");
    TH1D *aa_hError_Zg_lowRg = (TH1D*) fileAA->Get("aa_hError_Zg_lowRg");
    
    TH1D *aa_hProj_Zg_highRg = (TH1D*) fileAA->Get("aa_hProj_Zg_highRg");
    TH1D *aa_hError_Zg_highRg = (TH1D*) fileAA->Get("aa_hError_Zg_highRg");
    
    //Create files from 1D PbPb projections
    create("theta_g","table5.yaml", aa_hProj_Rg_lowZg, aa_hError_Rg_lowZg, "0.2-0.35", "zg_RANGE","Pb–Pb","0-10 percent");
    create("theta_g","table6.yaml", aa_hProj_Rg_highZg, aa_hError_Rg_highZg, "0.35-0.5", "zg_RANGE","Pb–Pb","0-10 percent");
    create("z_g","table7.yaml", aa_hProj_Zg_lowRg, aa_hError_Zg_lowRg, "0.0-0.3", "thetag_RANGE","Pb–Pb","0-10 percent");
    create("z_g","table8.yaml", aa_hProj_Zg_highRg, aa_hError_Zg_highRg, "0.3-1.0", "thetag_RANGE","Pb–Pb","0-10 percent");

    //Files for pp
    TFile *filePP = new TFile("Vac/Projections.root", "READ");
    //Rg
    TH1D *pp_hProj_Rg_lowZg = (TH1D*) filePP->Get("pp_hProj_Rg_lowZg");
    TH1D *pp_hError_Rg_lowZg = (TH1D*) filePP->Get("pp_hError_Rg_lowZg");
    
    TH1D *pp_hProj_Rg_highZg = (TH1D*) filePP->Get("pp_hProj_Rg_highZg");
    TH1D *pp_hError_Rg_highZg = (TH1D*) filePP->Get("pp_hError_Rg_highZg");
    
    //Zg
    TH1D *pp_hProj_Zg_lowRg = (TH1D*) filePP->Get("pp_hProj_Zg_lowRg");
    TH1D *pp_hError_Zg_lowRg = (TH1D*) filePP->Get("pp_hError_Zg_lowRg");
    
    TH1D *pp_hProj_Zg_highRg = (TH1D*) filePP->Get("pp_hProj_Zg_highRg");
    TH1D *pp_hError_Zg_highRg = (TH1D*) filePP->Get("pp_hError_Zg_highRg");

    //Create files from 1D pp projections
    create("theta_g","table1.yaml", pp_hProj_Rg_lowZg, pp_hError_Rg_lowZg, "0.2-0.35", "zg_RANGE","pp","none");
    create("theta_g","table2.yaml", pp_hProj_Rg_highZg, pp_hError_Rg_highZg, "0.35-0.5", "zg_RANGE","pp","none");
    create("z_g","table3.yaml", pp_hProj_Zg_lowRg, pp_hError_Zg_lowRg, "0.0-0.3", "thetag_RANGE","pp","none");
    create("z_g","table4.yaml", pp_hProj_Zg_highRg, pp_hError_Zg_highRg, "0.3-1.0", "thetag_RANGE","pp","none");

    //Ratio histograms
    TH1D *ratioRg_lowZg;
    TH1D *ratioErrRg_lowZg;
    ratioRg_lowZg = make_ratio(aa_hProj_Rg_lowZg, pp_hProj_Rg_lowZg);
    ratioErrRg_lowZg = make_ratio(aa_hError_Rg_lowZg, pp_hError_Rg_lowZg);
    create("theta_g","table9.yaml", ratioRg_lowZg, ratioErrRg_lowZg, "0.2-0.35", "zg_RANGE","Pb–Pb/pp","0-10 percent");

    TH1D *ratioRg_highZg;
    TH1D *ratioErrRg_highZg;
    ratioRg_highZg = make_ratio(aa_hProj_Rg_highZg, pp_hProj_Rg_highZg);
    ratioErrRg_highZg = make_ratio(aa_hError_Rg_highZg, pp_hError_Rg_highZg);
    create("theta_g","table10.yaml", ratioRg_highZg, ratioErrRg_highZg, "0.35-0.5", "zg_RANGE","Pb–Pb/pp","0-10 percent");

    TH1D *ratioZg_lowRg;
    TH1D *ratioErrZg_lowRg;
    ratioZg_lowRg = make_ratio(aa_hProj_Zg_lowRg, pp_hProj_Zg_lowRg);
    ratioErrZg_lowRg = make_ratio(aa_hError_Zg_lowRg, pp_hError_Zg_lowRg);
    create("z_g","table11.yaml", ratioZg_lowRg, ratioErrZg_lowRg, "0.0-0.3", "thetag_RANGE","Pb–Pb/pp","0-10 percent");

    TH1D *ratioZg_highRg;
    TH1D *ratioErrZg_highRg;
    ratioZg_highRg = make_ratio(aa_hProj_Zg_highRg, pp_hProj_Zg_highRg);
    ratioErrZg_highRg = make_ratio(aa_hError_Zg_highRg, pp_hError_Zg_highRg);
    create("z_g","table12.yaml", ratioZg_highRg, ratioErrZg_highRg, "0.3-1.0", "thetag_RANGE","Pb–Pb/pp","0-10 percent");

    TH1D *ratioTheta_HightoLowZg_pp;
    TH1D *ratioErrTheta_HightoLowZg_pp;
    ratioTheta_HightoLowZg_pp = make_ratio(pp_hProj_Rg_highZg, pp_hProj_Rg_lowZg);
    ratioErrTheta_HightoLowZg_pp = make_ratio(pp_hError_Rg_highZg, pp_hError_Rg_lowZg);
    create("theta_g","table13.yaml", ratioTheta_HightoLowZg_pp, ratioErrTheta_HightoLowZg_pp, "0.35-0.5/0.2-0.35", "zg_RANGE","(pp)_highZ/(pp)_lowZ","none");

    TH1D *ratioTheta_HightoLowZg_aa;
    TH1D *ratioErrTheta_HightoLowZg_aa;
    ratioTheta_HightoLowZg_aa = make_ratio(aa_hProj_Rg_highZg, aa_hProj_Rg_lowZg);
    ratioErrTheta_HightoLowZg_aa = make_ratio(aa_hError_Rg_highZg, aa_hError_Rg_lowZg);
    create("theta_g","table14.yaml", ratioTheta_HightoLowZg_aa, ratioErrTheta_HightoLowZg_aa, "0.35-0.5/0.2-0.35", "zg_RANGE","(Pb–Pb)_highZ/(Pb–Pb)_lowZ","0-10 percent");


    TH1D *doubleRatioRg;
    TH1D *doubleRatioErrRg;
    doubleRatioRg = make_ratio(ratioRg_highZg, ratioRg_lowZg);
    doubleRatioErrRg = make_ratio(ratioErrRg_highZg, ratioErrRg_lowZg);
    create("theta_g","table15.yaml", doubleRatioRg, doubleRatioErrRg, "0.35-0.5/0.2-0.35", "zg_RANGE","(Pb–Pb/pp)_highZ/(Pb–Pb/pp)_lowZ","0-10 percent");

    //File for 2D correlations
    TFile *fileUnfPP = new TFile("LHC17_unf.root", "READ");//pp
    TH3D *DataCorrelationPP = (TH3D*) fileUnfPP->Get("hData_Unf_N3");
    TFile *fileUnfPbPb = new TFile("LHC18.root", "READ");//PbPb
    TH3D *DataCorrelationPbPb = (TH3D*) fileUnfPbPb->Get("hData_Unf_N3");
    

}