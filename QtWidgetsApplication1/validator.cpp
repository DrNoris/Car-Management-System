#include "validator.h"

void ValidatorMasini::validate(const Masina& m) {
	std::vector<string> msgs;
	if (m.getNumarInm().size() < 6) msgs.push_back("NUMAR DE INMATRICULAR INVALID!!!\n");
	if (m.getModel().size() < 3) msgs.push_back("MODEL INVALID!!!\n");
	if (m.getTip().size() < 3) msgs.push_back("TIP INVALID!!!\n");
	if (m.getProducator().size() < 3) msgs.push_back("PRODUCATOR INVALID!!!\n");

	if (msgs.size() > 0) {
		throw Exception(msgs);
	}
}

ostream& operator<<(ostream& out, const Exception& ex) {
	for (const auto& msg : ex.msgs) {
		out << msg << "\n";
	}
	return out;
}