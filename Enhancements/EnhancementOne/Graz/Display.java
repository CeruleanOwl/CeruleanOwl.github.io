/*
 * Author: Wesley Roberts
 * Date: 5/19/2024
*/
package Graz;
import java.util.ArrayList;

public class Display {
    

    //Displays a welcome message with Company name in artistic ASCII font
    public static void welcomeMessage() {

        System.out.println(" ██████  ██████   █████  ███████ ██  ██████  ███████  ██████      ███████  █████  ██      ██    ██  █████  ██████  ███████     ███    ███ ███████ ███    ██ ██    ██ \r\n" + //
                           "██       ██   ██ ██   ██    ███  ██ ██    ██ ██      ██    ██     ██      ██   ██ ██      ██    ██ ██   ██ ██   ██ ██          ████  ████ ██      ████   ██ ██    ██ \r\n" + //
                           "██   ███ ██████  ███████   ███   ██ ██    ██ ███████ ██    ██     ███████ ███████ ██      ██    ██ ███████ ██████  █████       ██ ████ ██ █████   ██ ██  ██ ██    ██ \r\n" + //
                           "██    ██ ██   ██ ██   ██  ███    ██ ██    ██      ██ ██    ██          ██ ██   ██ ██       ██  ██  ██   ██ ██   ██ ██          ██  ██  ██ ██      ██  ██ ██ ██    ██ \r\n" + //
                           " ██████  ██   ██ ██   ██ ███████ ██  ██████  ███████  ██████      ███████ ██   ██ ███████   ████   ██   ██ ██   ██ ███████     ██      ██ ███████ ██   ████  ██████ ");
    }
    
    
    
    
    // Prints the menu options to the user
    public static void displayMenu() {
        System.out.println(
                            "\n\n" +
                            "\t\t\t\t╔═══════════════════════════════════════════════════════╗\n" +
                            "\t\t\t\t║ Rescue Animal System Menu                             ║\n" +
                            "\t\t\t\t╠═══════════════════════════════════════════════════════╣\n" +
                            "\t\t\t\t║ [1] Intake a new dog                                  ║\n" +
                            "\t\t\t\t║ [2] Intake a new monkey                               ║\n" +
                            "\t\t\t\t║ [3] Intake a new cat                                  ║\n" +
                            "\t\t\t\t║ [4] Reserve an animal                                 ║\n" +
                            "\t\t\t\t║ [5] Print a list of all dogs                          ║\n" +
                            "\t\t\t\t║ [6] Print a list of all monkeys                       ║\n" +
                            "\t\t\t\t║ [7] Print a list of all cats                          ║\n" +
                            "\t\t\t\t║ [8] Print a list of all animals that are not reserved ║\n" +
                            "\t\t\t\t║ [q] Quit application                                  ║\n" +
                            "\t\t\t\t╚═══════════════════════════════════════════════════════╝\n" +
                            "\n" +
                            "Enter a menu selection:");
    }

        // The printAnimals() method has four different outputs
        // Based on the listType parameter:
        // dog - prints the list of all dogs
        // Monkeys - prints the list of all monkeys
        // Cats - prints a list of all cats
        // available - prints a combined list of all animals that are fully trained ("in service") but not reserved (false) and "None" if none match the criteria
        public static void printAnimals(String listType, ArrayList<Dog> dogList, ArrayList<Monkey> monkeyList, ArrayList<Cat> catList) {
            if (listType == "Dogs") {
                System.out.println("╔════════════════════════════════════════════════╗\n" +
                                   "║List of all Dogs                                ║\n" +
                                   "╚════════════════════════════════════════════════╝");
            	for (Dog dog : dogList) {
            		System.out.println("Name: " + dog.getName() + ", Status: " + dog.getTrainingStatus() + ", Acquisition Country: " + dog.getAcquisitionLocation() + ", Reserved: " + dog.getReserved());
            	}
            }
            else if(listType == "Monkeys") {
                System.out.println("╔════════════════════════════════════════════════╗\n" +
                                   "║List of all Monkeys                             ║\n" +
                                   "╚════════════════════════════════════════════════╝");
            	for (Monkey monkey : monkeyList) {
            		System.out.println("Name: " + monkey.getName() + ", Status: " + monkey.getTrainingStatus() + ", Acquisition Country: " + monkey.getAcquisitionLocation() + ", Reserved: " + monkey.getReserved());
            	}
            }
            else if(listType == "Cats") {
                System.out.println("╔════════════════════════════════════════════════╗\n" +
                                   "║List of all Cats                                ║\n" +
                                   "╚════════════════════════════════════════════════╝");
            	for (Cat cat : catList) {
            		System.out.println("Name: " + cat.getName() + ", Status: " + cat.getTrainingStatus() + ", Acquisition Country: " + cat.getAcquisitionLocation() + ", Reserved: " + cat.getReserved());
            	}
            }
            else {
            	System.out.println("╔════════════════════════════════════════════════╗\n" +
                                   "║Available dogs in service, but not reserved:    ║\n" +
                                   "╚════════════════════════════════════════════════╝");
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
            	
            	System.out.println("╔════════════════════════════════════════════════╗\n" +
                                   "║Available monkeys in service, but not reserved: ║\n" +
                                   "╚════════════════════════════════════════════════╝");
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

                System.out.println("╔════════════════════════════════════════════════╗\n" +
                                   "║Available cats in service, but not reserved:    ║\n" +
                                   "╚════════════════════════════════════════════════╝");
            	int catCounter = 0; //Checks if any dog meets requirements, if not gives a new output.
            	for (Cat cat : catList) {
            		if (!cat.getReserved() && (cat.getTrainingStatus().equalsIgnoreCase("in Service"))) {
            			System.out.println("Name: " + cat.getName() + ", Status: " + cat.getTrainingStatus() + ", Acquisition Country: " + cat.getAcquisitionLocation() + ", Reserved: " + cat.getReserved());
            			catCounter++;
            		}
            	}
                if (catCounter == 0) {
            		System.out.println("None");
            	}
            }

        }
}
