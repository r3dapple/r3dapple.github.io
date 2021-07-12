#include "rsa.hh"

rsa_keypair::rsa_keypair(std::string public_key) : keySize(0), e(0), n(0), d(0), name("encrypt_only") {
	initializePublic(public_key);
}

void rsa_keypair::initializePublic(std::string public_key) {

	std::istringstream iss(public_key);
	std::string tempS;
	std::vector<std::string> v;
	while (getline(iss, tempS, '.')) {
		v.push_back(tempS);
	}
	
	// https://stackoverflow.com/a/6868389/9298528
	// placement new
	(&n)->~n();
	new (&n) BigInteger(v[0]);
	
	e = atoi(v[1].c_str());

	keySize = (int)std::pow(2, v[0].size());

	std::cout << "Initialized " << name << " for encryption only!\n" << std::endl;
}

rsa_keypair::rsa_keypair() : keySize(0), e(0), n(0), d(0), name("unnamed") {}

rsa_keypair::rsa_keypair(int keySize, int e = 3, const char *name = "myKeys") : keySize(keySize), e(e), name(name){

	if(keySize % 128 != 0){
		throw "Invalid key size. Use keySize % 128 = 0";
	}

	if(std::strlen(name) > 90){
		throw "Name too large";
	}

	p = getLargePrimeP(keySize);
	q = getLargePrimeQ(keySize);
	n = p * q;

	phi = ((p-1)*(q-1))/gcd(p-1, q-1);

	while(1){
		try{
			d = modinv(e,phi);
			break;
		}catch(const char* ex){
			std::cout << ex << std::endl;
			if(e == 3) e = 5;
			else if(e == 5) e = 17;
			else if(e == 17) e = 257;
			else if(e == 257) e = 65537;
			else e++;
			// phi cant share a factor with e
			std::cout << "Increasing e to " << e << std::endl;
			std::cout << std::endl;
		}
	}

	std::cout << "n = " << n << std::endl;
	std::cout << "phi = " << phi << std::endl;
	std::cout << "d = " << d << std::endl;
	std::cout << "e = " << e << std::endl;
	std::cout << std::endl;
}

BigInteger rsa_keypair::encrypt(BigInteger x){
    if(x >= n) {return BigInteger();}
    return modpow(x,e,n);
}

void rsa_keypair::save(){

    char public_file_name[100];
    strcpy(public_file_name, name);
    strcat(public_file_name, "_public.txt");

    char private_file_name[100];
    strcpy(private_file_name, name);
    strcat(private_file_name, "_private.txt");

    std::ofstream public_file(public_file_name, std::ios::out);
    std::string public_key_string = n.getNumber() + "." + std::to_string(e);
    public_file << public_key_string;
    public_file.close();

    std::ofstream private_file(private_file_name, std::ios::out);
    std::string private_key_string = n.getNumber() + "." + std::to_string(e) + "." + d.getNumber();
    private_file << private_key_string;
    private_file.close();

    std::cout << "Saved " << name << "!\n" << std::endl;
}

void rsa_keypair::load(std::string private_key_file){

    std::ifstream private_file(private_key_file, std::ios::in);
    std::string private_key_string;
    private_file >> private_key_string;
	private_file.close();
	
	loadPrivateFromString(private_key_string);
}

void rsa_keypair::loadPrivateFromString(std::string private_key){

	std::string tempS;
    std::istringstream iss(private_key);
    std::vector<std::string> v;
    while(getline(iss, tempS, '.')){
        v.push_back(tempS);
    }

    (&n)->~n();
	new (&n) BigInteger(v[0]);
	
    e = atoi(v[1].c_str());

	(&d)->~d();
	new (&d) BigInteger(v[2]);
	
	keySize = (int)std::pow(2, v[0].size());
	
    std::cout << "Loaded " << name << "!\n" << std::endl;
}

BigInteger rsa_keypair::decrypt(BigInteger x){
    return modpow(x,d,n);
}
