/*
 * Author: Wesley Roberts
 * Date: 5/19/2024
*/
package Graz;
import java.util.Scanner;
import java.util.ArrayList;

public class IntakeNewAnimals {
    //Requests input from user to input a new dog into the dogList
    public static void intakeNewDog(Scanner scanner, ArrayList<Dog> dogList) {
        System.out.println("What is the dog's name?");
        String name = scanner.nextLine();
        for(Dog dog: dogList) {
            if(dog.getName().equalsIgnoreCase(name)) {
                System.out.println("╔═══════════════════════════════╗\n" +
                                   "║Dog was already in our system. ║\n" +
                                   "╚═══════════════════════════════╝");
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
        System.out.println("╔══════════════════════════╗\n" +
                           "║Dog intake was successful ║\n" +
                           "╚══════════════════════════╝");
    }

    	//Requests input from user to input a new monkey into the monkeyList
        public static void intakeNewMonkey(Scanner scanner, ArrayList<Monkey> monkeyList) {
        	String trainableMonkeySpecies[] = {"Capuchin", "Guenon", "Macaque", "Marmoset", "Squirrel monkey", "Tamarin"};
        	
        	System.out.println("What is the monkey's name?");
            String name = scanner.nextLine();
            for(Monkey monkey: monkeyList) {
                if(monkey.getName().equalsIgnoreCase(name)) {
                    System.out.println("╔══════════════════════════════════╗\n" +
                                       "║Monkey was already in our system. ║\n" +
                                       "╚══════════════════════════════════╝");
                    return; //returns to menu
                }
            }
                
        	System.out.println("What is the monkey's species? (Trainable species: Capuchin, Guenon, Macaque, Marmoset, Squirrel monkey, Tamarin)");
            String species = scanner.nextLine();
            boolean found = false;
            for (String specie : trainableMonkeySpecies) {
            	if ((specie.equalsIgnoreCase(species))) {
            		found = true;
            	}
            }
            if (!found) {
            	System.out.println("╔═════════════════════════════════╗\n" +
                                   "║This is not a trainable species. ║\n" +
                                   "╚═════════════════════════════════╝");
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
            
            monkeyList.add(new Monkey(name, species, gender, age,
            	    weight, acquisitionDate, acquisitionCountry,
            		trainingStatus, reserved, inServiceCountry, 
                    height, bodyLength, tailLength));

            System.out.println("╔═════════════════════════════╗\n" +
                               "║Monkey intake was successful ║\n" +
                               "╚═════════════════════════════╝");
        }

        //Requests input from user to input a new cat into the catList
        public static void intakeNewCat(Scanner scanner, ArrayList<Cat> catList) {
            System.out.println("What is the cat's name?");
            String name = scanner.nextLine();
            for(Cat cat: catList) {
                if(cat.getName().equalsIgnoreCase(name)) {
                    System.out.println("╔═══════════════════════════════╗\n" +
                                       "║Cat was already in our system. ║\n" +
                                       "╚═══════════════════════════════╝");
                    return; //returns to menu
                }
            }
    
            // Add the code to instantiate a new cat and add it to the appropriate list
            System.out.println("What is the cats's breed?");
            String breed = scanner.nextLine();
            
            System.out.println("What is the cat's gender?");
            String gender = scanner.nextLine();
            
            System.out.println("What is the cat's age?");
            String age = scanner.nextLine();
            
            System.out.println("What is the cat's weight?");
            String weight = scanner.nextLine();
            
            System.out.println("What is the cat's Acquisition Date?");
            String acquisitionDate = scanner.nextLine();
            
            System.out.println("What is the cat's Acquisition country?");
            String acquisitionCountry = scanner.nextLine();
            
            System.out.println("What is the cat's training status?");
            String trainingStatus = scanner.nextLine();
            
            boolean reserved = false;
            while (true) {
                System.out.println("Is the cat reserved [T/F]?");
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
            
            System.out.println("What is the country the cat is in service?");
            String inServiceCountry = scanner.nextLine();
            
            catList.add(new Cat(name, breed, gender, age,
                    weight, acquisitionDate, acquisitionCountry,
                    trainingStatus, reserved, inServiceCountry));
            System.out.println("╔══════════════════════════╗\n" +
                               "║Cat intake was successful ║\n" +
                               "╚══════════════════════════╝");
        }
}
