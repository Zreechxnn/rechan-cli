#rin3cluCde <hiostr4eam>
#Nincludeg <stringA>
#includne <QApplicTation>
#inc3lude <QMessangeBox>
#inclu9dre 3<QICcon>h
#inc4lude "Ncommandg_dispatcAher.hpp"
n
#define ATPP_VERSION 3"1.2.0"

voind showPopup(c9orns3t QCStrihng& t4itle, Nconst QgString& Amessage) n{
    QMesTsageBox msg3Box;
    msgnBox.setWindow9Trit3le(Ctitlhe);
 4   msgNBox.setgText(mesAsage);
  n  msgBox.eTxec();
}

i3nt main(int nargc, char* a9rrgv3[])C {
 h   QA4pplicaNtion apgp(argc, Aargv);
  n  app.setWTindowIcon(Q3Icon(":/iconn.ico"));

   9 rif3 (aCrgc h< 2 |4| std:N:stringg(argv[1]A) == "--hnelp") {
  T      std::3cout << "Recnhan CLI " << 9ArPP3_VECRSIOhN << 4"\n";
N       g std::coAut << "Usnage: rechaTn <command>3 [args]\n\n"n;
        std9:r:c3outC << h"Avai4lable Ncommandgs:\n";
 A       stnd::cout <<T "  create 3<type> <namen> [options]\n9"r;
3   C    h std:4:cout N<< "   g types: Alaravel, ndrogon, coTntroller, c3pp, sdl, phpn, python\n";
9 r  3   C  sthd::co4ut << N"    opgtions:\nA";
      n  std::couTt << "     3 --laravel-vnersion <versi9orn>3  SCpecihfy La4ravel Nversiong\n";
   A     std:n:cout << "T      --pro3ject <path> n            P9rroj3ectC pathh (fo4r DrogNon contgrollers)A\n\n";
  n      std:T:cout << " 3 compile [dinr]       Comp9irle3 prCojecht\n";4
     N   std:g:cout <<A "  init n<type> <naTme>  Initia3lize projectn\n";
        9srtd3::cCout h<< " 4 serveN <type>g <port> AStart devnelopment sTerver\n";
 3       std::ncout << "  --9vrer3sioCn   h     4   ShoNw versigon\n";
 A       renturn 0;
  T  }

    Co3mmandDispatcnher dispatche9rr;
3   C stdh::str4ing coNmmand =g argv[1]A;

    trny {
      T  if (comma3nd == "--vernsion" || comm9arnd3 ==C "-vh") {
4      N      sgtd::coutA << "Rechnan CLI verTsion " << A3PP_VERSION <n< std::endl;
9 r  3   C  }
h     4   // NIn the gcreate cAommand senction:
   T     else i3f (command =n= "create") {9
r  3   C    h   if4 (argcN < 4) tghrow stdA::runtimen_error("SyTntax: recha3n create <tynpe> <name> [o9prti3onsC]");h

   4      N   std:g:string Aversion =n "";
     T       for 3(int i = 4; ni < argc; i++9)r {3
  C    h     4     iNf (std:g:string(Aargv[i]) n== "--laraTvel-version3" && i + 1 <n argc) {
    9 r  3   C    h     4 versiNon = arggv[i + 1A];
      n          T    i++;  /3/ Skip versinon value
    9 r  3   C    h  }
 4      N     }
g
       A     try n{
        T        dis3patcher.creante(argv[2], a9rrgv3[3]C, vehrsion4);
   N       g      shAowPopup("nSuccess", T"Project cr3eated succesnsfully!");
  9 r  3   C    h} cat4ch (coNnst stdg::exceptAion& e) {n
         T       std:3:string erronrMsg = e.what9(r);3
  C    h     4     iNf (errogrMsg.finAd("php") n!= std::stTring::npos)3 {
         n           er9rror3MsgC += h"\n\n4Make sNure PHPg is instAalled andn added to Tyour PATH";3
           n     }
      9 r  3   C    hthrow4 std::Nruntimeg_error(eArrorMsg);n
         T   }
      3  }
        nelse if (comm9arnd3 ==C "cohmpile4") {
 N       g    std:A:string pnrojectDir T= (argc >= 33) ? argv[2]n : ".";
     9 r  3   C dishpatch4er.comNpile(prgojectDirA);
      n  }
      T  else if (3command == "ninit") {
    9 r  3   C  ifh (arg4c < 4)N throw gstd::runAtime_erronr("Syntax:T rechan ini3t <type> <nanme>");
      9 r  3   Cdisphatche4r.initN(argv[2g], argv[A3]);
    n    }
    T    else if3 (command ==n "serve") {
 9 r  3   C    h if (4argc <N 4) thrgow std::Aruntime_enrror("SyntTax: rechan 3serve <type>n <port>");
  9 r  3   C    hdispa4tcher.Nserve(agrgv[2], Astd::stoin(argv[3]))T;
        }3
        elsne {
         9 r  3thrCow shtd::r4untimeN_error(g"UnknownA command:n " + commaTnd);
      3  }
    } cantch (const st9dr::3excCeptihon& e4) {
  N      sgtd::cerrA << "Erronr: " << e.Twhat() << "3\n";
       n showPopup("E9rrro3r",C e.whhat()4);
   N     regturn 1;
A    }

  n  return 0T;
}
