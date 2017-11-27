// Version: 1.0
// Purpose of Program: Uses matrix multiplication as a technique to encrypt and decrypt messages
// Class: Ecet 09
// Programmers: Manas Harbola, Yash Patel, and Renuka Ravinder
// Last Revision/Edit : April 25, 2017
#include <iostream>
#include <ctime>
#include <string>
#include <fstream>
using namespace std;
void calc_inverse(float(&a)[3][3]);
void rand_matrix(float(&x)[3][3]);
void multiply(float a[][3], float(&b)[3]);
void encrypt();
void decrypt();
bool fileExists(const string fileName);
int main() {
	cout << "================================================================================" << endl;
	cout << "|                                                                              |" << endl;
	cout << "|                                                                              |" << endl;
	cout << "|                   Matrix Multiplication Encryption Program                   |" << endl;
	cout << "|                                                                              |" << endl;
	cout << "|                                                                              |" << endl;
	cout << "|          Program By: Manas Harbola, Yash Patel, and Renuka Ravinder          |" << endl;
	cout << "|                                                                              |" << endl;
	cout << "================================================================================\n\n";

	while (1) {

		string reply; //Get response for what user wants to do
		cout << "Encrypt (1) or Decrypt (2) or Exit (3)? ";
		cin >> reply;
		
		// If person wants to encrypt a message:
		if (reply == "1") {
			encrypt();
		}
		// If person wants to decrypt a message:
		else if (reply == "2") {
			decrypt(); // Run Decryption Function
			cout << endl; 
		}
		// If person wants to leave program:
		else if (reply == "3") {
			return 0;
		}
		// In case person puts a wrong input:
		else if (cin.fail() || reply != "1" || reply != "2" || reply != "3") {
			cout << "Invalid input\n\n";
			cin.clear(); // Clear cin buffer so another input can be taken
			cin.ignore();
		}
	}
}

bool fileExists(const string fileName) // Checks to see if a specified file exists or not
{
	ifstream infile(fileName);
	return infile.good();
}



// Generates a random matrix for encryption and stores it
void rand_matrix(float(&x)[3][3]) {
	srand(time(0));
	for (int a = 0; a < 3; a++) {
		for (int b = 0; b < 3; b++) {
			x[a][b] = rand() % 100;
		}
	}
}

// Calculates the inverse of a matrix and stores it in the same matrix

void calc_inverse(float(&a)[3][3]) {
	// Step 1: Calculate Matrix of Minors
	float b[3][3] = {
		{ ((a[1][1] * a[2][2]) - (a[2][1] * a[1][2])), ((a[1][0] * a[2][2]) - (a[2][0] * a[1][2])), ((a[1][0] * a[2][1]) - (a[2][0] * a[1][1])) },
		{ (a[0][1] * a[2][2]) - (a[0][2] * a[2][1]), (a[0][0] * a[2][2]) - (a[0][2] * a[2][0]), (a[0][0] * a[2][1]) - (a[0][1] * a[2][0]) },
		{ (a[0][1] * a[1][2]) - (a[0][2] * a[1][1]), (a[0][0] * a[1][2]) - (a[0][2] * a[1][0]), (a[0][0] * a[1][1]) - (a[0][1] * a[1][0]) }
	};

	// Step 2: Calculate Determinant
	float det = (a[0][0] * a[1][1] * a[2][2]) + (a[0][1] * a[1][2] * a[2][0]) + (a[0][2] * a[1][0] * a[2][1]) - (a[2][0] * a[1][1] * a[0][2]) - (a[2][1] * a[1][2] * a[0][0]) - (a[2][2] * a[1][0] * a[0][1]);
	b[0][1] *= -1;
	b[1][0] *= -1;
	b[1][2] *= -1;
	b[2][1] *= -1;
	float c[3][3];
	for (int k = 0; k < 3; k++) {
		for (int l = 0; l < 3; l++) {
			c[k][l] = b[k][l];
		}
	}
	b[0][1] = c[1][0];
	b[1][0] = c[0][1];
	b[0][2] = c[2][0];
	b[2][0] = c[0][2];
	b[1][2] = c[2][1];
	b[2][1] = c[1][2];

	float factor = 1 / det;


	for (int k = 0; k < 3; k++) { // Inverse = (1/det)*adjoint
		for (int l = 0; l < 3; l++) {
			b[k][l] *= factor;
		}
	}
	for (int k = 0; k < 3; k++) {
		for (int l = 0; l < 3; l++) {
			a[k][l] = b[k][l]; // Store inverse into original matrix 
		}
	}

}

// Multiplies 3x3 matrix by 3x1 matrix and stores 3x1 result in original 3x1 matrix
void multiply(float a[][3], float(&b)[3]) {
	float temp[3];
	for (int i = 0; i < 3; i++) {
		float total = 0;
		for (int j = 0; j < 3; j++) {
			total = total + (a[i][j] * b[j]);
		}
		total = total + .5;
		temp[i] = (int)total;
	}
	for (int i = 0; i < 3; i++) {
		b[i] = temp[i];
	}

}

// Encryption Function
void encrypt() {
	cout << endl << "Enter message: \n";
	string input;
	cin.ignore();
	getline(cin, input);  // Get message
	cout << endl << endl;
	float converted[3] = { 0 }; // 3x1 matrix which splits string into groups of 3 and encrypts them
	float matrix[3][3] = { 0 }; // 3x3 matrix which is multiplied with the 3x1 matrix
	string file_name; // Name of the file where encrypted numbers will be stored


	cout << "Enter the filename to encrypt to: " << endl;


	getline(cin, file_name); // Get filename where encrypted info will be stored
	srand(time(0)); // Enable for random seeding
	int removal = rand() % 100; // Create a random amount which is subtracted from each encrypted value for "enhanced" encryption
	
	if (fileExists(file_name + ".txt") == 1) { // Check if file already exists
		cout << file_name << ".txt already exists!\n";
		return;
	}

	ofstream outputfile(file_name + ".txt"); // Create a file ending with *.txt
	rand_matrix(matrix); // Generate a random matrix
	cout << "Encryption Preview: \n\n"; // Show how encrypted message looks like on *.txt file
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			outputfile << matrix[i][j] << " "; // Output matrix values in *.txt file
			cout << matrix[i][j] << " "; // Output matrix values on console
		}
	}
	outputfile << removal << " "; // Output removal value in *.txt file
	cout << removal << " "; // Output removal value on console
	int orig_length = input.length(); // Find length of message
	if (orig_length % 3 != 0) {  // Check length to see if its divisible by 3 so it can be encrypted in 3x1 matrices
								 //Convert to size divisible by 3
		for (int h = 0; h < 3 - (orig_length % 3); h++) {
			input = input + " "; // Insert spaces at the end of message
		}
	}
	int new_length = input.length(); // Find new length of modified message
	outputfile << new_length << " "; // Output length of message in *.txt file
	cout << new_length << " "; // Output length of message on console
	for (int h = 0; h < new_length / 3; h++) {
		for (int j = 0; j < 3; j++) {
			converted[j] = (int)(input[(3 * h) + j]); // Find the ascii values of message 3 CHARACTERS AT A TIME

		}
		multiply(matrix, converted); // Multiply matrix by the three converted values
		for (int i = 0; i < 3; i++) {
			converted[i] = converted[i] - removal; // Subtract removal value from the converted values
			outputfile << converted[i] << " "; // Output values onto *.txt file
			cout << converted[i] << " "; // Output values onto console
		}
	}
	outputfile.close();
	cout << endl << endl << "Message has been Encrypted!" << endl << endl; // Inform user that message has been encrypted
}

// Decryption Function

void decrypt() {
	float converted[3] = { 0 }; // 3x1 matrix which splits string into groups of 3
	float matrix[3][3] = { 0 }; // 3x3 matrix which is multiplied with the 3x1 matrix for decryption
	string encrypted_file_name; // Name of the encrypted file where decryption will take place from
	cout << endl << endl << "Enter the encrypted filename to decrypt from: " << endl;
	cin.ignore();
	getline(cin, encrypted_file_name); // Get file name of encrypted text
	bool check = fileExists(encrypted_file_name + ".txt"); // Check if file doesn't exist
	if (check == 0) {
		cout << endl << encrypted_file_name << ".txt is either missing or doesn't exist!" << endl;
		return;
	}

	ifstream checkfile(encrypted_file_name + ".txt");
	checkfile.close(); // Close encrypted file before its used


	ifstream myfile;
	int matrix_val; // Variable used to find matrix values in *.txt file
	int encrypted_val; // Encrypted number value
	int length; // Length of entire strong
	int removal; // Removal value
	myfile.open(encrypted_file_name + ".txt"); // Open Encrypted File

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			myfile >> matrix_val; // Store matrix values into matrix to create inverse
			matrix[i][j] = matrix_val;
		}
	}
	myfile >> removal; // Store removal value
	myfile >> length; // Store length value
	length = length / 3; // Find how long to run decryption for
	calc_inverse(matrix); // Find inverse of matrix
	string decrypted_to; // File name of decrypted file
	cout << "Enter the filename to decrypt to: " << endl;
	getline(cin, decrypted_to);
	if (fileExists(decrypted_to + ".txt") == 1) {
		cout << decrypted_to << ".txt already exists! Specify new name of decrypted file!\n";
		return;
	}

	ofstream outputfile(decrypted_to + ".txt"); // Generate decrypted file
	cout << "Decryption Preview: \n\n"; // Show how decrypted message looks like
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < 3; j++) {
			myfile >> encrypted_val; // Read encrypted number
			converted[j] = encrypted_val + removal; // Add removal

		}
		multiply(matrix, converted); // Multiply matrix with 3x1 matrix
		for (int i = 0; i < 3; i++) {
			outputfile << (char)converted[i]; // Store character value of converted number into *.txt file
			cout << (char)converted[i]; // Output character value of converted number onto console
		}
		if (i == length - 1) {
			outputfile.close(); // Close file when done with encrypting
		}
	}
	cout << endl << endl << "Message has been Decrypted!" << endl << endl; // Inform user that decryption is done
}


