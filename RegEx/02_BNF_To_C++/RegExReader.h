#pragma once

class RegExReader {
	public:
		RegExReader();

		void addKeywords() {
		}

		bool def();
		bool MORE_EXPRESSION();
		bool CHARACTER_CLASS();
		bool PARSE_PRIMARY();
		bool PARSE_QUANTIFIERS();
		bool PARSE_FACTOR();
		bool PARSE_TERM();
		bool PARSE_EXPRESSION();

		
		void 													read(const char* sFile);
	protected:							
									
	private:							

};
