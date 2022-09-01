#include <iostream>
#include <random>
#include <string>

std::random_device rd;
std::mt19937 rng(rd());
std::uniform_int_distribution<> distrib(75, 125);
std::bernoulli_distribution fiftyFifty(0.5);
std::bernoulli_distribution trigger(0.35);

class Character{
	protected:
		static const short maxPoint = 300;
		long power, defense, damage;
		float lifeSteal = 0.0;	
		
	public:
		long long hp;
		bool isBoss = false;
		bool blockState = false;
		std::string name;
		
		Character(std::string x){
			name = x;
			hp = distrib(rng);
			power = distrib(rng);
			defense = maxPoint - hp - power;
			hp *= 10;
			power *= 2;
			defense *= 2;
		}
		void showStat(){
			std::cout << "---------- (" << name << ") \n";
			std::cout << "Hp       : " << hp << '\n';
			std::cout << "Power    : " << power << '\n';
			std::cout << "Defense  : " << defense << '\n';
			std::cout << "Lifesteal: " << lifeSteal * 100 << "% \n";
			std::cout << "---------- \n";
		}
		void attack(Character &target){
			damage = power * exp((double) -target.defense/power * 0.69);
			if (damage == 0){
				power *= 1.5;
				std::cout << name << " mendapatkan anti-meta blessing! Meningkatkan power sebesar 50% untuk ronde selanjutnya \n";
			}
			if (target.blockState){
				std::cout << target.name << " memblokir. Damage dikurangi sebesar 60% \n";
				damage *= 0.4;
				if (isBoss || trigger(rng)){
					power *= 0.95;
					std::cout << target.name << " berhasil mengurangi power " << name << " sebesar 5% \n";  
				}
				if (fiftyFifty(rng)){
					hp -=  damage;
					std::cout << target.name << " berhasil membalikkan serangan! menghasilkan damage sebesar " << damage << '\n';					
				} else {
					target.hp -= damage;
					std::cout << name << " menyerang! menghasilkan damage sebesar " << damage << '\n';
				}
				target.blockState = false;
			} else {
				target.hp -= damage;
				std::cout << name << " menyerang! Menghasilkan damage sebesar " << damage << '\n'; 
				
			}
			if (lifeSteal > 0.0){
				long heal = damage * lifeSteal;
				hp += heal;
				std::cout << name << " memulihkan hp sebesar " << heal << '\n'; 
			}
			if (trigger(rng)){
				power = (int) power * 1.15;
				std::cout << name << " berhasil meningkatkan power sebesar 15%! \n";
			}
		}
		void block(){
			std::cout << name << " melakukan blokir! \n";
			if (trigger(rng)){
				defense *= 1.4;
				std::cout << name << " berhasil meningkatkan def sebesar 40%! \n";
			}
			blockState = true;
		}
		void heal(){
			if (fiftyFifty(rng)){
				hp *= 1.2;
				std::cout << name << " melakukan heal! meningkatkan hp sebesar 20% \n";
			} else {
				lifeSteal += 0.05;
				std::cout << name << " gagal melakukan heal. Meningkatkan lifesteal sebesar 5%\n";
			}
			if (trigger(rng)){
				lifeSteal += 0.1;
				std::cout << name << " berhasil meningkatkan lifesteal sebesar 10% \n";
			}	
		}
};


void battleScreen(Character &ally, Character &enemy){
	using namespace std;
	int option;
	while(ally.hp > 0 && enemy.hp > 0){
		ally.showStat();
		enemy.showStat();
		cout << "Pilih opsi: \n"
				"(1) Serang (2) Blokir (3) heal (4) Kabur \n"
				"-> ";
		cin >> option;
		switch(option){
			case 1:
				ally.attack(enemy);
				break;
			case 2:
				ally.block();
				break;
			case 3:
				ally.heal();
				break;
			case 4:
				return;
			default:
				cout << "Pilihan anda tidak valid \n";
				continue;
		}
		if(trigger(rng) && !enemy.blockState) enemy.block();
		else if (trigger(rng)) enemy.heal();
		else enemy.attack(ally);
	}
	if(enemy.hp <= 0) cout << ally.name << " menang! \n";
	else cout << enemy.name << " menang! \n";
	cout << "-------------------- \n";
}


void mainMenu(bool &done){
	using namespace std;
	int option;
	cout << "---------------------------------\n"
			"|  Selamat datang di game gelud |\n"
			"|  Pilih opsi:                  |\n"
			"|  1. Mulai                     |\n"
			"|  2. Panduan                   |\n"
			"|  3. Keluar                    |\n"
			"---------------------------------\n"
			"-> ";	
	cin >> option;
	Character player("Pemain");
	Character enemy ("Musuh");
	if (fiftyFifty(rng)){
		enemy = Character("Boss");
		enemy.hp *= 7.7;
		enemy.isBoss = true;
	}
	else {
		enemy.hp *= 1.5;
	}
	bool recreated = true;
	switch(option){
		case 1:
			cout << "Menciptakan karakter anda... \n";
			while(recreated){
				player.showStat();
				cout << "Apakah anda sudah puas dengan stat anda? \n"
						"1. Ya \n"
						"2. Tidak (buat lagi) \n"
						"-> ";
				cin >> option;
				if(option == 2){
					player = Character("Pemain");
					continue;
				} else {
					recreated = false;
				}
			
			}
			battleScreen(player, enemy);
			cout << "Apakah anda ingin bermain lagi? \n"
					"1. Ya \n"
					"2. Tidak \n"
					"-> ";
			cin >> option;
			if (option != 1) done = true;
			break;
		case 2:
			cout << "Dalam pertarungan akan ada 4 pilihan yaitu serang, blokir, heal dan kabur \n"
					"Pilihan serang akan menghasilkan damage sesuai dengan kalkulasi power anda dan defense musuh dan memiliki peluang untuk meningkatkan power \n"
					"Pilihan blokir akan mengurangi damage yang diterima sebesar 50% dan memiliki 50% kemungkinan untuk mengembalikan damage tersebut dan memiliki peluang untuk meningkatkan defense \n"
					"Pilihan heal memiliki 50% kemungkinan untuk meningkatkan hp. Jika gagal maka meningkatkan lifesteal sebesar 5% \n"
					"musuh akan secara acak memilih aksi blokir atau serang sehingga berkreasilah untuk membuat meta \n";
			break;
		case 3:
			done = true;
			break;
		default:
			cout << "Pilihan anda tidak valid \n";
	}
}


int main(){
	bool done = false;
	while(!done){
		mainMenu(done);
	}
}