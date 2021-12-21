using Library;
public class DemAccount : Account
{
    public DemandAccount(decimal sum, int percentage) : base(sum, percentage)
    {
    }

    protected internal override void Open()
    {
        base.OnOpened(new AccountEventArgs($"Открыт новый счет! Id счета: {this.Id}", this.Sum));
    }
}