/*
 * Author: Wesley Roberts
 * Date: 4/4/2021
*/
package Graz;
import java.util.ArrayList;
import java.util.Scanner;

public class Driver {
    private static ArrayList<Dog> dogList = new ArrayList<Dog>();
    // Instance variables (if needed) 
    
    private static ArrayList<Monkey> monkeyList = new ArrayList<Monkey>();

    public static void main(String[] args) {
    	String userInput = "";

    	Scanner scnr = new Scanner(System.in);
        initializeDogList();
        initializeMonkeyList();

        // Accepts user input with given parameters
        while (!userInput.equalsIgnoreCase("q")) {
        	displayMenu();
        	userInput = scnr.nextLine();
        	
        	switch (userInput) {
        	case "1":
        		intakeNewDog(scnr);
        		break;
        	case "2":
        		intakeNewMonkey(scnr);
        		break;
        	case "3":
        		reserveAnimal(scnr);
        		break;
        	case "4":
        		printAnimals("Dogs");
        		break;
        	case "5":
        		printAnimals("Monkeys");
        		break;
        	case "6":
        		printAnimals("Not Reserved");
        		break;
        	case "q":
        		break;
        	default:
        		System.out.println("Please enter a valid input.");	
        	}

        }

    }

    // This method prints the menu options
    public static void displayMenu() {
        System.out.println("\n\n");
        System.out.println("\t\t\t\tRescue Animal System Menu");
        System.out.println("[1] Intake a new dog");
        System.out.println("[2] Intake a new monkey");
        System.out.println("[3] Reserve an animal");
        System.out.println("[4] Print a list of all dogs");
        System.out.println("[5] Print a list of all monkeys");
        System.out.println("[6] Print a list of all animals that are not reserved");
        System.out.println("[q] Quit application");
        System.out.println();
        System.out.println("Enter a menu selection");
    }


    // Adds dogs to a list for testing
    public static void initializeDogList() {
        Dog dog1 = new Dog("Spot", "German Shepherd", "male", "1", "25.6", "05-12-2019", "United States", "intake", false, "United States");
        Dog dog2 = new Dog("Rex", "Great Dane", "male", "3", "35.2", "02-03-2020", "United States", "Phase I", false, "United States");
        Dog dog3 = new Dog("Bella", "Chihuahua", "female", "4", "25.6", "12-12-2019", "Canada", "in service", true, "Canada");

        dogList.add(dog1);
        dogList.add(dog2);
        dogList.add(dog3);
    }


    // Adds monkeys to a list for testing
    //Optional for testing
    public static void initializeMonkeyList() {
        Monkey monkey1 = new Monkey("boots", "Guenon", "male", "5", "25.6", "05-12-2019", "United States", "intake", false, "United States", "45\"", "25\"", "20\"");
        Monkey monkey2 = new Monkey("Bobby Brown", "Tamarin", "male", "3", "35.2", "02-03-2020", "United States", "Phase I", false, "United States", "45\"", "25\"", "20\"");
        Monkey monkey3 = new Monkey("Kolton", "Squirrel monkey", "female", "4", "25.6", "12-12-2019", "Canada", "in service", true, "Canada", "45\"", "25\"", "20\"");

        monkeyList.add(monkey1);
        monkeyList.add(monkey2);
        monkeyList.add(monkey3);
    }

    // The input validation to check that the dog is not already in the list
    // is done for you
    public static void intakeNewDog(Scanner scanner) {
        System.out.println("What is the dog's name?");
        String name = scanner.nextLine();
        System.out.println("here");
        for(Dog dog: dogList) {
            if(dog.getName().equalsIgnoreCase(name)) {
                System.out.println("\n\nThis dog is already in our system\n\n");
                return; //returns to menu
            }
        }

        // Add the code to instantiate a new dog and add it to the appropriate list
        System.out.println("What is the dog's breed?");
        String breed = scanner.nextLine();
        
        System.out.println("What is the dog's gender?");
        String gender = scanner.nextLine();
        
        System.out.println("What is the dog's age?");
        String age = scanner.nextLine();
        
        System.out.println("What is the dog's weight?");
        String weight = scanner.nextLine();
        
        System.out.println("What is the dog's Acquisition Date?");
        String acquisitionDate = scanner.nextLine();
        
        System.out.println("What is the dog's Acquisition country?");
        String acquisitionCountry = scanner.nextLine();
        
        System.out.println("What is the dog's training status?");
        String trainingStatus = scanner.nextLine();
        
        boolean reserved = false;
        while (true) {
        	System.out.println("Is the dog reserved [T/F]?");
        	String tf = scanner.nextLine();
        	
        	if (tf.equalsIgnoreCase("t")) {
        		reserved = true;
        		break;
        	}
        	else if(tf.equalsIgnoreCase("f")) {
        		reserved = false;
        		break;
        	}
        	else {
        		System.out.println("Please enter either T or F");
        	}
        }
        
        System.out.println("What is the country the dog is in service?");
        String inServiceCountry = scanner.nextLine();
        
        dogList.add(new Dog(name, breed, gender, age,
        	    weight, acquisitionDate, acquisitionCountry,
        		trainingStatus, reserved, inServiceCountry));
    }

    	//checks if monkey name and species are appropriate for adding to list and allow user to input rest of information after the checks pass.
        public static void intakeNewMonkey(Scanner scanner) {
        	String trainableMonkeySpecies[] = {"Capuchin", "Guenon", "Macaque", "Marmoset", "Squirrel monkey", "Tamarin"};
        	
        	System.out.println("What is the monkey's name?");
            String name = scanner.nextLine();
            for(Monkey monkey: monkeyList) {
                if(monkey.getName().equalsIgnoreCase(name)) {
                    System.out.println("\n\nThis monkey is already in our system\n\n");
                    return; //returns to menu
                }
            }
                
        	System.out.println("What is the monkey's species?");
            String species = scanner.nextLine();
            boolean found = false;
            for (String specie : trainableMonkeySpecies) {
            	if ((specie.equalsIgnoreCase(species))) {
            		found = true;
            	}
            }
            if (!found) {
            	System.out.println("This is not a trainable species.");
        		return;
            }
            
            System.out.println("What is the monkey's gender?");
            String gender = scanner.nextLine();
            
            System.out.println("What is the monkey's age?");
            String age = scanner.nextLine();
            
            System.out.println("What is the monkey's weight?");
            String weight = scanner.nextLine();
            
            System.out.println("What is the monkey's acquisition date?");
            String acquisitionDate = scanner.nextLine();
            
            System.out.println("What is the monkey's acquisition country?");
            String acquisitionCountry = scanner.nextLine();
            
            System.out.println("What is the monkey's training status? (If in service, don't include hyphen)");
            String trainingStatus = scanner.nextLine();
            
            boolean reserved = false;
            while (true) {
            	System.out.println("Is the monkey reserved [T/F]?");
            	String tf = scanner.nextLine();
            	
            	if (tf.equalsIgnoreCase("t")) {
            		reserved = true;
            		break;
            	}
            	else if(tf.equalsIgnoreCase("f")) {
            		reserved = false;
            		break;
            	}
            	else {
            		System.out.println("Please enter either T or F");
            	}
            }
            
            System.out.println("What is the monkey's in service country?");
            String inServiceCountry = scanner.nextLine();
            
            System.out.println("What is the monkey's height?");
            String height = scanner.nextLine();
            
            System.out.println("What is the monkey's body length?");
            String bodyLength = scanner.nextLine();
            
            System.out.println("What is the monkey's tail length?");
            String tailLength = scanner.nextLine();
            
            monkeyList.add(new Monkey(name,species, gender, age,
            	    weight, acquisitionDate, acquisitionCountry,
            		trainingStatus, reserved, inServiceCountry, height, bodyLength, tailLength));
        }

        // Complete reserveAnimal
        // You will need to find the animal by animal type and in service country //include name because of edge cases
        public static void reserveAnimal(Scanner scanner) {
            System.out.println("What is the animal's name?");
            String name = scanner.nextLine();
            String type0 = "";
            
            while (true) {
	            System.out.println("What is the type of animal? (Monkey or Dog)");
	            String type = scanner.nextLine();
	            if (type.equalsIgnoreCase("Monkey") || (type.equalsIgnoreCase("Dog"))) {
	            	type0 = type;
	            	break;
	            }
	            else {
	            	System.out.println("Not a type.");
	            }
            }
            
            System.out.println("What is the service country of the animal?");
            String inServiceCountry = scanner.nextLine();
            
            if (type0.equalsIgnoreCase("monkey")) {
            	int monkeyChecker = 0; //Checks if the information never passed conditions.
            	for (Monkey monkey : monkeyList) {
            		if (monkey.getName().equalsIgnoreCase(name) && (monkey.getInServiceLocation().equalsIgnoreCase(inServiceCountry)))  {
            			monkey.setReserved(true);
            			monkeyChecker++;
            		}
            	}
        		if (monkeyChecker == 0) {
        			System.out.println("Could not find monkey by that name and service country.");
        		}
            }
            
            if (type0.equalsIgnoreCase("dog")) {
            	int dogChecker = 0;
            	for (Dog dog : dogList) {
            		if (dog.getName().equalsIgnoreCase(name) && (dog.getInServiceLocation().equalsIgnoreCase(inServiceCountry)))  {
            			dog.setReserved(true);
            			dogChecker++;
            		}
            		if (dogChecker == 0) {
            			System.out.println("Could not find dog by that name and service country.");
            		}
            	}
            }
            	
            

        }

        // The printAnimals() method has three different outputs
        // based on the listType parameter
        // dog - prints the list of dogs
        // monkey - prints the list of monkeys
        // available - prints a combined list of all animals that are
        // fully trained ("in service") but not reserved 
        public static void printAnimals(String listType) {
            if (listType == "Dogs") {
            	for (Dog dog : dogList) {
            		System.out.println("Name: " + dog.getName() + ", Status: " + dog.getTrainingStatus() + ", Acquisition Country: " + dog.getAcquisitionLocation() + ", Reserved: " + dog.getReserved());
            	}
            }
            else if(listType == "Monkeys") {
            	for (Monkey monkey : monkeyList) {
            		System.out.println("Name: " + monkey.getName() + ", Status: " + monkey.getTrainingStatus() + ", Acquisition Country: " + monkey.getAcquisitionLocation() + ", Reserved: " + monkey.getReserved());
            	}
            }
            else {
            	System.out.println("Available dogs in service, but not reserved: ");
            	int dogCounter = 0; //Checks if any dog meets requirements, if not gives a new output.
            	for (Dog dog : dogList) {
            		if (!dog.getReserved() && (dog.getTrainingStatus().equalsIgnoreCase("in Service"))) {
            			System.out.println("Name: " + dog.getName() + ", Status: " + dog.getTrainingStatus() + ", Acquisition Country: " + dog.getAcquisitionLocation() + ", Reserved: " + dog.getReserved());
            			dogCounter++;
            		}
            	}
            	if (dogCounter == 0) {
            		System.out.println("None");
            	}
            	
            	System.out.println("Available monkeys in service, but not reserved: ");
            	int monkeyCounter = 0; //Checks if any monkey meets requirements, if not gives a new output.
            	for (Monkey monkey : monkeyList) {
            		if (!monkey.getReserved() && (monkey.getTrainingStatus().equalsIgnoreCase("in Service"))) {
            			System.out.println("Name: " + monkey.getName() + ", Status: " + monkey.getTrainingStatus() + ", Acquisition Country: " + monkey.getAcquisitionLocation() + ", Reserved: " + monkey.getReserved());
            			monkeyCounter++;
            		}
            	}
            	if (monkeyCounter == 0) {
            		System.out.println("None");
            	}
            }

        }

}

