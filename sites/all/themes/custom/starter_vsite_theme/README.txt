To create your own theme for vsites:

1. Copy this entire directory

2. Rename your new directory to the name of your new theme (example: my_theme)

3. In that new my_theme directory, rename the .css and .info files so they read my_theme.info, and my_theme.css, respectively.

4. Open the .info file with a text editor and change references to 'starter_vsite_theme' to 'my_theme'. Note: As you can select whether your theme is public (i.e. can be used by everyone on this installation) or private, choosing the name of the theme is important. Private themes must be named for the vsite they are designed for. For example if you are creating a site for a Professor Bob Smith, who has the domain 'bsmith', and want the theme to be only available to Bob Smith, you should name the theme 'bsmith'.

5. Open the .css file and change as you wish. (Peruse the theming documentation on http://openscholar.harvard.edu to avoid excessive nashing of teeth.)

5. Go to /admin/build/themes/select and enable your new custom theme.



To create flavors for your theme


1. Create a directory called "flavors"
2. Inside "flavors" directory create a directory for each flavor (e.g. "myflavor")
3. Under "myflavor" directory create the .flav file . 
   The name of the file should be [theme name]_[flavor name].flv . For example if your theme name is scholar_aglet, 
   and you want to create a flavor called "myflavor" , then the name of the file should be scholar_aglet_myflavor.flav

   
Note: In the same way you can create flavors for another theme.  For example, if you want "myflavor" to be a flavor of 
scholar_aglet theme, then specify this in .flav file of your flavor.

Please read comments at flavors/myflavor/starter_vsite_theme_myflavor.flav 