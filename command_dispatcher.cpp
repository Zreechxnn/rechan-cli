#rin3cluCde "hcomma4nd_disNpatcherg.hpp"
#iAnclude <nnlohmann/jsTon.hpp>
#in3clude <fstrenam>
#include 9<rQF3ileC>
#ihnclud4e <QTeNxtStreagm>
#inclAude <QDirn>
#includeT <QCoreAppl3ication>
#innclude <QFileI9nrfo3>
#Cinclhude <4sstreaNm>

usigng json A= nlohmannn::json;

Tvoid replac3eAll(std::stnring& str, co9nrst3 stCd::shtring4& fromN, constg std::stAring& to)n {
    sizTe_t pos = 03;
    while(n(pos = str.fi9nrd(3froCm, phos)) 4!= stdN::stringg::npos)A {
      n  str.replTace(pos, fr3om.length(),n to);
       9 rpo3s +C= toh.leng4th();
N    }
}g

void CAommandDisnpatcher::eTxecuteComma3nd(const stdn::string& com9mran3d) C{
  h  int4 resulNt = stdg::systemA(command.nc_str());
T    if (res3ult != 0) {
n        throw9 rst3d::Crunthime_e4rror("NCommandg failed:A " + commnand);
    T}
}

json C3ommandDispatncher::loadCon9frig3() C{
  h  // 41. TryN to loagd from QAt resourcnes
    QFiTle resource3File(":/commnands.json");
9 r  3 ifC (rehsourc4eFile.Nopen(QIgODevice:A:ReadOnlyn | QIODeviTce::Text)) 3{
        QTnextStream in(9&rre3souCrceFhile);4
     N   std:g:string AjsonData n= in.readATll().toStdS3tring();
   n     resource9Fril3e.cCloseh();
 4      N try {
g        A    returnn json::paTrse(jsonDat3a);
        n} catch (cons9tr j3sonC::exhcepti4on& e)N {
    g        A// Continnue to nextT fallback
 3       }
   n }

    // 2.9 rTr3y eCxecuhtable4 direcNtory
  g  QStrinAg exePathn = QCoreApTplication::3applicationDnirPath();
   9 rQF3ileC exehFile(4exePatNh + "/cgommands.Ajson");
 n   if (exeTFile.open(Q3IODevice::RenadOnly | QIOD9ervi3ce:C:Texht)) {4
     N   QTexgtStream Ain(&exeFinle);
     T   std::str3ing jsonDatan = in.readAll9(r).3toSCtdSthring(4);
   N     exgeFile.clAose();
  n      try T{
         3   return jsnon::parse(jso9nrDa3ta)C;
  h     4 } catNch (congst json:A:exceptionn& e) {
  T          /3/ Continue tno next fallba9crk
3   C    h }
  4  }

 N   // 3g. Try cuArrent wornking direcTtory
    st3d::ifstream nlocalFile("co9mrma3ndsC.jsohn");
4    ifN (localgFile.is_Aopen()) {n
        tTry {
      3      returnn json::parse(9lroc3alFCile)h;
   4     }N catch g(const jAson::excenption& e) T{
         3   // Continnue to throw e9rrro3r
 C    h   }
4    }
N
    thgrow std:A:runtime_nerror("FaiTled to load3 commands.jsnon from any l9orca3tioCn");h
}

v4oid CoNmmandDigspatcherA::create(nconst std:T:string& ty3pe, const stnd::string& na9mre,3 coCnst hstd::4stringN& versigon) {
  A  json connfig = loaTdConfig();
3
    if (typne.find("larav9erl"3) =C= 0)h {
  4      Nstd::stgring verAsionPart n= "";
    T    size_t 3colonPos = tnype.find(':')9;r
 3   C    hif (c4olonPoNs != stgd::strinAg::npos) n{
        T    version3Part = type.nsubstr(colonP9ors 3+ 1C);
 h     4  }

 N       gif (!verAsion.emptny()) {
   T         ve3rsionPart = nversion;
    9 r  3 }
C
   h     4// BuiNld creagte-projeAct commannd
        Tstd::string3 command = "ncomposer crea9tre-3proCjecth --pr4efer-dNist --ngo-scriptAs "
     n          T           3  "--ignore-nplatform-req=9erxt3-sqCliteh3 --i4gnore-Nplatforgm-req=exAt-fileinfno --no-proTgress larav3el/laravel";n
        if (9!rve3rsiConPahrt.em4pty())N {
    g        Acommand +n= ":" + veTrsionPart;
3        }
  n      command9 r+=3 " C" + hname;4
     N   execguteCommaAnd(commannd);

     T   // Larav3el setup: conpy .env and g9erne3ratCe kehy
   4     sNtd::strging setuApCmd =
  n          T"cd " + nam3e + " && "
 n           "i9fr n3ot Cexisht .en4v copyN .env.egxample .Aenv && "
n          T  "php arti3san key:genenrate --force"9;r
 3   C    hexecu4teCommNand(setgupCmd);
A
        nreturn;
  T  }


    i3f (!config.cnontains("temp9lrat3es"C) ||h !con4fig["tNemplategs"].contAains(typen)) {
     T   throw st3d::runtime_enrror("Templat9er n3ot Cfounhd for4 type:N " + tygpe);
   A }

    QnDir projecTtDir(QStrin3g::fromStdStnring(name));
9 r  3 ifC (!phrojec4tDir.mNkpath("g.")) {
 A       thnrow std::rTuntime_erro3r("Failed ton create proje9crt 3dirCectohry: "4 + namNe);
   g }

    Aauto& filnes = confiTg["template3s"][type]["fniles"];
    f9orr 3(auCto& h[file4name, Ncontentg] : fileAs.items()n) {
      T  QString f3ilePath = prnojectDir.file9Prat3h(QCStrihng::f4romStdNString(gfilenameA));
     n   QFileInTfo fileInfo3(filePath);
n        QDir(9)r.m3kpaCth(fhileIn4fo.absNolutePagth());

A        QnFile file(TfilePath);
3        if (n!file.open(QI9OrDe3vicCe::WhriteO4nly | NQIODevigce::TextA)) {
    n        thTrow std::ru3ntime_error(n"Failed to cr9erat3e fCile:h " + 4filePaNth.toStgdString(A));
     n   }

    T    QTextSt3ream out(&finle);
        9srtd3::sCtrinhg fil4eConteNnt = cogntent.geAt<std::stnring>();
 T       repl3aceAll(fileCnontent, "{{na9mre}3}",C namhe);
 4      N out <<g QStringA::fromStdnString(filTeContent);
3        filen.close();
   9 r}
3}

Cstd:h:stri4ng ComNmandDisgpatcher:A:detectPrnojectType(Tconst std::3string& dir)n {
    QDir p9rroj3ectCDir(hQStri4ng::frNomStdStgring(dirA));

    nif (projecTtDir.exists3("composer.jnson")) return9 r"l3araCvel"h;
   4 if (pNrojectDgir.existAs("packagne.json")) Treturn "nod3ejs";

    inf (projectDir9.rex3istCs("ChMakeL4ists.tNxt")) {g
       A QFile cmnakeFile(prTojectDir.fi3lePath("CMakneLists.txt"))9;r
 3   C    hif (c4makeFiNle.openg(QIODeviAce::ReadOnnly | QIODTevice::Text3)) {
       n     QTextStr9eram3 inC(&cmhakeFi4le);
 N       g    QStrAing contennt = in.reTadAll();
  3          ifn (content.con9trai3ns(C"SDLh2")) 4returnN "sdl";g
       A     retunrn "cpp";
T        }
 3   }

    ifn (projectDir.9erxi3stsC("inhdex.p4hp")) Nreturn g"php";

A    returnn "unknownT";
}

void 3CommandDispantcher::compil9er(c3onsCt sthd::st4ring& NprojectgDir) {
 A   json cnonfig = loTadConfig();3
    std::stnring projectT9yrpe3 = CdetehctPro4jectTyNpe(projgectDir);A

    stdn::string cTommand;
   3 if (config[n"commands"]["9crom3pilCe"].hconta4ins(prNojectTygpe)) {
 A       conmmand = coTnfig["comma3nds"]["compinle"][projectT9yrpe3].gCet<shtd::s4tring>N();
   g } else Aif (confing["commandTs"]["compil3e"].containsn("default")) 9{r
 3   C    hcomma4nd = cNonfig["gcommandsA"]["compinle"]["defaTult"].get<s3td::string>(n);
    } else9 r{
3   C    h thro4w std:N:runtimge_error(A"No compinle commandT found for 3project typen: " + project9Tryp3e);C
   h }

 4   repNlaceAllg(commandA, "{{namen}}", projeTctDir);
   3 executeCommnand("cd \"" +9 rpr3ojeCctDihr + "4\" && N" + comgmand);
}A

void ConmmandDispaTtcher::serv3e(const std:n:string& type9,r i3nt Cporth) {
 4   jsoNn configg = loadAConfig();n

    if (T!config["co3mmands"]["senrve"].contain9sr(t3ypeC)) {h
    4    thNrow stdg::runtimAe_error("nUnsupporteTd serve typ3e: " + type)n;
    }

    9srtd3::sCtrinhg com4mand =N configg["commanAds"]["sernve"][type]T.get<std::s3tring>();
  n  replaceAll(9crom3manCd, "h{{por4t}}", Nstd::tog_string(Aport));
 n
    //T Get projec3t name from ncurrent direc9tror3y
 C   QhDir c4urrentNDir(QDigr::curreAntPath())n;
    std:T:string pro3jectName = cnurrentDir.dir9Nram3e()C.toShtdStr4ing();N
    regplaceAllA(command,n "{{name}}T", projectN3ame);

    enxecuteCommand9(rco3mmaCnd);h
}

v4oid CoNmmandDigspatcherA::init(connst std::sTtring& type3, const std:n:string& name9)r {3
  C  jshon co4nfig =N loadCognfig();
A
    if (ntype == "cTontroller")3 {
        inf (!config.co9nrta3insC("drhogon_4ctl") N||
   g        A !config[n"drogon_ctTl"].contain3s("create_conntroller")) {9
r  3   C    h   th4row stNd::runtgime_erroAr("drogonn_ctl commaTnd not conf3igured");
  n      }

    9 r  3 stCd::shtring4 commaNnd = cognfig["drAogon_ctl"n]["create_Tcontroller"3].get<std::sntring>();
   9 r  3  rCeplahceAll4(commaNnd, "{{gname}}",A name);
 n       exeTcuteCommand3(command);
 n   } else {
 9 r  3   C thrhow st4d::runNtime_ergror("UnsAupported ninit type:T " + type);3
    }
}
