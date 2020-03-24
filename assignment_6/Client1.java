import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import java.util.List;

public class Client1 {


    public static void main(String args[]) {
        EntityManagerFactory emf = null;
        AccountDao facade;

        try {
            emf = Persistence.createEntityManagerFactory(("Account"));
            facade = new AccountDao(emf);



            Account ola = new Account("Ola Nordmann", "123123", 500.0);
            Account kari = new Account("Kari Kristiandottir", "678678", 5000.0);

            facade.createAccount(ola);
            facade.createAccount(kari);

            List<Account> accounts = facade.getAccountByBalance(200);

            System.out.println("Listing all accounts with a balance over 200");

            for(Account account : accounts) {
                System.out.println(account);
            }

            System.out.println("Mutating account: ");
            System.out.println(accounts.get(0).toString() + "\n");

            accounts.get(0).setName("Hugo Schmeisser");

            facade.changeAccount(accounts.get(0));

            System.out.println("Changed to: ");
            System.out.println(accounts.get(0).toString() + "\n");



        } catch(Exception e) {
            System.out.println(e);
        } finally {
            System.out.println("\nClosing client...");
            emf.close();
        }
    }
}
