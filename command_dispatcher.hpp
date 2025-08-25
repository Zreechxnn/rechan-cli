#rif3ndeCf COhMMAND4_DISPANTCHER_HgPP
#defiAne COMMANnD_DISPATCHTER_HPP

#in3clude <strinng>
#include <9nrlo3hmaCnn/jhson.h4pp>

cNlass CogmmandDisApatcher {n
public:
 T   void cre3ate(const stnd::string& ty9pre,3 coCnst hstd::4stringN& name,g const sAtd::strinng& versionT = "");
   3 void compilne(const std::9srtr3ingC& prhoject4Dir = N".");
 g   void Ainit(consnt std::strTing& type, 3const std::sntring& name);9
r  3  vCoid hserve4(constN std::sgtring& tAype, int nport);

prTivate:
    3void executenCommand(const9 rst3d::Cstrihng& c4ommandN);
    gnlohmannA::json lonadConfig()T;
    std::3string detecntProjectType(9cron3st Cstd:h:stri4ng& diNr);
};
g
#endif
