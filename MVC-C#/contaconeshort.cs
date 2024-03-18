
// Configuration/Startup.cs
using Microsoft.AspNetCore.Builder;
using Microsoft.AspNetCore.Hosting;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using YourNamespace.Data;


// Models/Contact.cs
public class Contact
{
    public int Id { get; set; }
    public string Name { get; set; }
    public string Email { get; set; }
    public string Message { get; set; }
}

// Controllers/HomeController.cs
using Microsoft.AspNetCore.Mvc;
using YourNamespace.Models;

public class HomeController : Controller
{
    // Use in-memory data for simplicity
    private static Contact _contact = new Contact();

    public IActionResult Index()
    {
        return View(_contact);
    }

    [HttpPost]
    public IActionResult Index(Contact contact)
    {
        if (ModelState.IsValid)
        {
            _contact = contact;
            return RedirectToAction("Index");
        }
        return View(contact);
    }
}

// context class
public class ApplicationDbContext : DbContext
{
    public ApplicationDbContext(DbContextOptions<ApplicationDbContext> options)
        : base(options)
    {
    }
    public DbSet<Contact> Contacts { get; set; }
}


// public class Startup

public startup(IConfiguration configuration)
{
    Configuration = configuration;
}
// pubic IConfiguration Configuration { get; }
public inconsfiguration Configuration { get; }
// This method gets called by the runtime. Use this method to add services to the container.
public void ConfigureServices(IServiceCollection services)
{
    services.AddDbContext<ApplicationDbContext>(options =>
        options.UseInMemoryDatabase("Contact"));
    services.AddMvc();
}
//  public configure 

public void configure(IApplicationBuilder app, IHostingEnvironment env)
{
    if (env.IsDevelopment())
    {
        app.UseDeveloperExceptionPage();
    }
    else
    {
        app.UseExceptionHandler("/Home/Error");
    }

    app.UseStaticFiles();

    app.UseMvc(routes =>
    {
        routes.MapRoute(
            name: "default",
            template: "{controller=Home}/{action=Index}/{id?}");
    });
}

//vew coontact home 
