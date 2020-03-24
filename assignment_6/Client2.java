import javax.persistence.EntityManagerFactory;
import javax.persistence.OptimisticLockException;
import javax.persistence.Persistence;
import javax.persistence.RollbackException;
import java.util.List;

public class Client2 {

    public static void main(String args[]) {
        EntityManagerFactory emf = null;
        AccountDao facade;

        try {
            emf = Persistence.createEntityManagerFactory("Account");
            facade = new AccountDao(emf);

            List<Account> accounts = facade.getAccountByBalance(200);

            accounts.get(0).deduct(100.0);
            accounts.get(1).add(100.0);

            // Thread.sleep(5000);

            facade.changeAccount(accounts.get(0));
            facade.changeAccount(accounts.get(1));
        } catch (OptimisticLockException|RollbackException e) {
            System.out.println(e);
        } finally {
            emf.close();
        }
    }
}
