#include <iostream>
#include <fstream>

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <plotWithRoot.h>
#include <RD53Style.h>


int main(int argc, char *argv[]) { // ./Jakob path/to/directory file_ext(png, pdf, C, root)
	
	
	//SetRD53Style();
	gStyle->SetTickLength(0.02);
	gStyle->SetTextFont();
	
	
	if (argc < 3) //User Input Error
	{
		std::cout << "No directory and/or file_ext given! " << std::endl;
		std::cout << "Example: ./plotWithRoot_Occupancy path/to/directory png" << std::endl;
		return -1;
	}
	
	
	DIR *ds; //Directory Stream
	struct dirent *dirp;
	struct stat filestat;
	
	std::string filename, filepath, chipnum; 
	std::string line1,line2,line3,line4;
	
	ds = opendir(argv[1]); //open directory
	
	
	if (ds == NULL) //check if directory is found
	{
		std::cout << "Directory not found." <<std::endl;
		return -1;
	}
	std::string dir = argv[1];
	
	//MAIN:
	
	while ((dirp = readdir(ds))) //pointer to structure representing directory entry at current position in directory stream, and positions directory stream at the next entry. Returns a null pointer when reaching the end of directory
	{							//readdir, counts /. and /.. as separate files
		filename = dirp->d_name;
		filepath = dir + "/" + filename; //whole filepath
		
		if (stat(filepath.c_str(), &filestat)) std::cout << "file valid" << std::endl;//continue;
		if (S_ISDIR(filestat.st_mode)) std::cout << "found Directory" << std::endl;//continue;
		std::cout << "file valid \n" + filepath << std::endl;
		
		if(strstr(filepath.c_str(), "OccupancyMap.dat") != NULL) //checks if string is contained in filename
		{
			std::cout << "Data found \n" << std::endl;
			
			chipnum = "Chip Nr: " + filename.substr(0, filename.find("_")); // get chipnumber from filename, from 0 to "_"
			
			std::cout << chipnum << std::endl;
			
			std::cout << "Opening file: " << filepath << std::endl;
			
			std::fstream infile(filepath.c_str(), std::ios::in); //open file for input
			
			
			std::getline(infile, line1);
			std::getline(infile, line2);
			std::getline(infile, line3);
			
			std::cout << line1 << "\n" << line2 << "\n" << line3 << "\n" << std::endl;
			
			/*for(int i = 0; i<200; i++)
			{
				double oc=0;
				infile >> oc;
				std::cout << oc << std::endl;
				//std::cout << line4 << std::endl;
			}*/
			
			bool array[192][400] = {0};
			for (int i = 0; i<192; i++)
			{
				for (int j = 0; j<400; j++)
				{
					double checkocc;
					infile >> checkocc;
					if(checkocc<10)
					{
						array[i][j]=1;
						std::cout << "Pixel mit occupancy < 10 : array ["<<i<<"]["<<j<<"]" << std::endl;
					}
				}
			}
			
			int array2[192][400] = {2};
			for (int i = 0; i<192; i++)
			{
				for (int j = 0; j<400; j++)
				{
					infile >> array2[i][j];
					
				}
			}
			
			
			
			
			//create 2-D Histogramm
			
			/*TH2 *h2d = NULL;
			h2d = (TH2*) new TH2I("2d", "2d", 400, 0,400, 192, 0, 192);
			
			//fill 2-D Histogramm
			
			for (int i = 0; i<192; i++)
			{
				for (int j = 0; j<400; j++)
				{
					h2d->SetBinContent(j,i,array2[i][j]);
					//std::cout << array2[i][j];
				}
			}*/
			
			//create Histogramm
			TH1 *testhisto = NULL;
			testhisto = (TH1*) new TH1F("test","test1",2,0,1);
			
			
			//fill histogramm
			for (int i = 0; i<192; i++)
			{
				for (int j = 0; j<400; j++)
				{
					if(array[i][j])
					{
						testhisto->AddBinContent(1);
					}
					else
					{
						testhisto->AddBinContent(2);
					}
				}
			}
			
			//formatting 2-D histogramm
			/*style_TH2(h2d, "x","y","z");
			
			
			//printing 2-D histogramm
			TCanvas *CC = new TCanvas("test","test", 1000,800);
			style_TH2canvas(CC);
			h2d->Draw();
			gStyle->SetPalette(kCandy);
			h2d->Draw("surf3");
			CC->Update();
			CC->Print("test.png");
			
			delete CC; 
			delete h2d;*/
			
			
		//formatting histogramm
		
		style_TH1(testhisto, "occupancy","Number of Pixels");
		testhisto->GetXaxis()->SetBinLabel(1,"<10");
		testhisto->GetXaxis()->SetBinLabel(2,">10");
		testhisto->GetXaxis()->LabelsOption("h");
		testhisto->SetMarkerSize(1);
		testhisto->SetMarkerColor(1);
		
		//Printing histogramm
		testhisto->SetFillColor(kOrange);
		testhisto->SetLineColor(kOrange);
		TCanvas *C = new TCanvas("c_test", "c_test", 1000,800);
		style_TH1canvas(C);
		testhisto->Draw();
		testhisto->Draw("TEXT0 SAME");
		TLatex *tname= new TLatex();
		latex_Chip(tname);
		tname->DrawLatex(0.28,0.96,"TEEST");
		tname->DrawLatex(0.8, 0.96, chipnum.c_str());
		TLegend *legend = new TLegend(0.7,0.82,0.88,0.91);
		legend->SetHeader("Legende", "A");
		legend->AddEntry(testhisto, "Legendeentry", "f");
		legend->SetBorderSize(0);
		legend->Draw();		
		testhisto->SetMaximum((testhisto->GetMaximum())*1.21);
		C->Update();
		std::string File = filename.replace(filename.find(".dat"),9,".png");
		C->Print(File.c_str());
		
		delete C;
		delete testhisto;
		
			
		}
		
		//3-D Histo
		
		//create Histogramm
		//TH3 *Histo3 = NULL;
		//Histo3 = (TH3*) new TH3I("TEST","TEEST", 192 , 0 , 200, 400,)
		
	}
	return 0;
}
