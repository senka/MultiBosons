import ROOT         # allows access to ROOT colors (e.g. ROOT.kRed)

##############################################################################
######## About Config Files ##################################################

## This file can be generated by running 'rootplotmpl --config'

## Options are loaded in the following order:
##   1. from the command line
##   2. from the default configuration file
##   3. from ~/.rootplotmplrc
##   4. from configuration files specified on the command-line
## This leads to two major points worth understanding:
##   1. you may delete any lines you like in this file and they will still
##      be loaded correctly from the default
##   2. values specified here will superced the same options from the
##      command-line
## Therefore, you could set, for example, 'xerr = True' in this file,
## and x-errorbars will always be drawn, regardless of whether '--xerr' is
## given on the command-line or not.  You can do this with any of the command-
## line options, but note that dashes are translated to underscores, so
## '--ratio-split=1' becomes 'ratio_split = 1'.

## Most global style options like default line widths can be set through
## a matplotlibrc, as described at:
##    http://matplotlib.sourceforge.net/users/customizing.html

##############################################################################
######## An Advanced Operation Example #######################################

#ratio_split=True
data=6
stack=True
xerr=True

## You can specify the files to run on through the 'filenames' variable rather
## than entering them at the command-line, for example:
filenames = ['analysisPlots_incmu_tight.root',
             'analysisPlots_ttbar_tight.root',
             'analysisPlots_wjets_tight.root',
             'analysisPlots_zjets_tight.root',
             'analysisPlots_zgamma_tight.root',
             'analysisPlots_data_tight.root']

## Then you could specify corresponding legend entries here rather than on the
## command-line:
legend_entries = ['QCD',
                  r'$t\bar{t}$',
                  'W+Jets',
                  'Z+Jets',
                  r'$Z\gamma$',
                  'Data']

intlum=1.33

scale=[intlum/(4345478./296900000.),
       intlum/(1483404./950.),
       intlum/(9968207./24170.),
       intlum/(954733./2400.),
       intlum/(100470./7.3),
       1]

##############################################################################
######## Different Options for Different Targets #############################

## Leave these lists empty to have them automatically filled according to the
## command-line options.  Any list that is filled must be at least as long
## as the number of targets or it will throw an error.
line_colors = []          # normally filled by options.colors
fill_colors = []          # normally filled by options.colors
marker_colors = []        # normally filled by options.colors
errorbar_colors = []      # color for bars around the central value
marker_sizes = []         # in points
line_styles = []          # 'solid', 'dashed', 'dashdot', 'dotted'
plot_styles = []          # 'bar', 'hist', 'errorbar', 'stack'
alphas = []               # transparencies for fills (value from 0 to 1)

##############################################################################
######## Global Style Options ################################################

## Colors can be specified as (r, g, b) tuples (with range 0. to 1. or range
## 0 to 255), ROOT color constants (ROOT.kBlue or 600), or any matplotlib
## color specification (names like 'blue' or 'b')

colors = [
    ## a default set of contrasting colors the author happens to like
    ( 82, 124, 219), # blue
    (212,  58, 143), # red
    (231, 139,  77), # orange
    (145,  83, 207), # purple
    (114, 173, 117), # green
    ( 67,  77,  83), # dark grey
    ]

## Used when --marker_styles is specified; more info available at:
## http://matplotlib.sourceforge.net/api/
##        artist_api.html#matplotlib.lines.Line2D.set_marker
marker_styles = [
    'o', 's', '^', 'x', '*', 'D', 'h', '1'
    ]

#### Styles for --data
data_color = (0,0,0)      # black
mc_color = (50, 150, 150) # used when there are exactly 2 targets; set to
                          # None to pick up the normal color
data_marker = 'o'         # marker style

#### Settings for --ratio-split or --efficiency-split

ratio_max = 1.5
ratio_min = -1.5
ratio_fraction = 0.3  # Fraction of the canvas that bottom plot occupies
ratio_label = 'Ratio to %(ratio_file)s' # Label for the bottom plot
efficiency_label = 'Efficiency vs. %(ratio_file)s'

#### Titles produced by --area-normalize and --normalize
area_normalized_title = 'Fraction of Events in Bin'
target_normalized_title = 'Events Normalized to %(norm_file)s'

#### Overflow and underflow text labels
overflow_text = ' Overflow'
underflow_text = ' Underflow'
overflow_size = 'small'
overflow_alpha = 0.5

#### Define how much headroom to add to the plot
top_padding_factor = 2.5
top_padding_factor_log = 100    # used when --logy is set

ymin=5e-4

#### Plotting options based on histogram names
## ex: set 'Logy' : ['_pt'] to make plots with _pt in the name be logy
## These options are the same as the 'sticky' plotting options in the help menu
options_by_histname = {'Norm' : [],   # area normalize
                       'Logx' : [],
                       'Logy' : ['Pt'],
                       'Zero' : ['Rapidity','Mass','Eta','Phi','Mt','Iso'],
                       'Overflow' : [],
                       'Underflow' : []}

## Experimental! This is a more generic form that is not limited to just the
## 'sticky' plotting options
options_by_histname2 = [('rebin', 10, 'Pt'),# set --rebin=2 for any hist with 'ecal' in name
                        ('xmin', 0, 'Pt'),
                        ('xmax', 100, 'Pt'),
                        ('rebin', 10, 'Eta'),
                        ('xmin', -4, 'Eta'),
                        ('xmax', 4, 'Eta'),
                        ('rebin', 20, 'Phi')
                       ]

#### Legend
#### These options will override legend_location, allowing more precise control
## Upper right corner of legend in figure coordinates
legend_figure_bbox = [1.0, 1.0] #for legend outside the axes
## Upper right corner of legend in axes coordinates
legend_axes_bbox = None

#### Page numbers
numbering_size_mpl = 'small'
numbering_ha_mpl = 'right'
numbering_va_mpl = 'top'
numbering_x_mpl = 0.98       # Fraction of canvas width
numbering_y_mpl = 0.98       # Fraction of canvas height

#### Rotation for text x-axis labels
xlabel_rotation = -15
xlabel_alignment = 'left'



#### Convert ROOT symbols to proper LaTeX, for matplotlib plotting
## By default, matplotlib renders only symbols between $'s as TeX, but if
## you enable the 'text.usetex' matplotlibrc setting, then everything is handled
## by the LaTeX engine on your system, in which case you can go wild with TeX.

## ROOT-type strings on left get replaced with LaTeX strings on the right
replace = [
    # some defaults that should work for most cases
    ('P_{#perp}', r' $p_\mathrm{T}$'),
    #(' P_{#perp} ', r' $p_\mathrm{T}$ '),
    #('P_{#perp} ', r'$p_\mathrm{T}$ '),
    #(' P_{#perp}', r' $p_\mathrm{T}$'),
    (' pt '    , r' $p_\mathrm{T}$ '),
    ('pT '     , r'$p_\mathrm{T}$ '),
    (' pT'     , r' $p_\mathrm{T}$'),
    ('p_{T}'   , r'$p_\mathrm{T}$'),
    ('E_{T}'   , r'$E_\mathrm{T}$'),
    ('M_{T}'   , r'$M_\mathrm{T}$'),
    ('#eta'    , r' $\eta$'),
    ('#phi'    , r' $\phi$'),
    ('fb^{-1}' , r'$\mathrm{fb}^{-1}$'),
    ('pb^{-1}' , r'$\mathrm{pb}^{-1}$'),
    ('nb^{-1}' , r'$\mathrm{nb}^{-1}$'),
    ('<'       , r'$<$'),
    ('>'       , r'$>$'),
    ('#Sigma'    , r'$\Sigma$'),
    ('#sigma'    , r'$\sigma$'),
    ('#'       , r''),
    ('[GeV/c^{2}]'   , r'$(GeV/c^{2})$'),
    ('[GeV/c]'   , r'$(GeV/c)$'),    
    ('[GeV]'     , r'$(GeV)$'),
    (' GeV/c^{2} '   , r' $GeV/c^{2}$ '),
    (' GeV/c '   , r' $GeV/c$ '),    
    (' GeV '     , r' $GeV$ '),
    (' GeV/c^{2}'   , r' $GeV/c^{2}$'),
    (' GeV/c'   , r' $GeV/c$'),    
    (' GeV'     , r' $GeV$'),
    ('[radians]' , r'(radians)'),    
    ('1 hardest' , r'Hardest'),
    ('2 hardest' , r'$2^{nd}$ Hardest'),
    ('3 hardest' , r'$3^{rd}$ Hardest'),
    ('4 hardest' , r'$4^{th}$ Hardest'),
    ('5 hardest' , r'$5^{th}$ Hardest'),
    ('1. hardest' , r'Hardest'),
    ('photon'    , r'Photon'),
    ('muon'      , r'Muon'),
    ('Daughter 0', r'VB Candidate'),
    (' per 1 '    , r' per 10 '), # to account for rebinning
    (' per 0.2' , r' per 2'),
    (' per 0.02' , r' per 0.2'),
    (' per 0.063 ',r' per 0.63 '),
    (' per .1',r' per 1'),
    ]

## If you include 'use_regexp' as the first item, the patterns to be replaced
## will function as regular expressions using python's re module rather than
## as simple text.  The example below turn's ROOT's superscript and subscript
## syntax into LaTeX:

## replace = [
##     ('use_regexp', True),
##     (r'\^\{(.*)\}', r'$^{\1}$'),
##     (r'\_\{(.*)\}', r'$_{\1}$'),
## ]

#### A function that will be executed after all histograms have been drawn.
#### It can be used to add extra decorations to your figure.
def decoration_mpl(figure, axeses, path, options, hists):
    #### Draw a line to indicate a cut
    ## axeses[0].axvline(5., color='red', linestyle='--')
    #### Add a caption
    figure.text(0.15, 0.85, "CMS Preliminary",size='large')
    figure.text(0.15, 0.75, r'$\int\,\mathcal{L}\ \mathrm{dt} = '+str(round(intlum,2))+' \mathrm{pb}^{-1}$',size='large')

    for hist in hists:
        titles=hist.title.split(":")
        hist.title=titles[0]
        if len(titles) == 2 :
            hist.xlabel=titles[1]
        if len(titles) == 3:
            hist.xlabel=titles[1]
            hist.ylabel=titles[2]        
        hist.show_titles(replacements=replace)

    return



##############################################################################
######## HTML Output #########################################################

#### Number of columns for images in HTML output
ncolumns_html = 2

#### Provide a template for the html index files
html_template=r'''
    <html>
    <head>
    <link rel='shortcut icon' href='http://packages.python.org/rootplot/_static/rootplot.ico'>
    <link href='http://fonts.googleapis.com/css?family=Yanone+Kaffeesatz:bold' rel='stylesheet' type='text/css'>
    <style type='text/css'>
        body { padding: 10px; font-family:Arial, Helvetica, sans-serif;
               font-size:15px; color:#FFF; font-size: large; 
               background-image: url(
                   'http://packages.python.org/rootplot/_static/tile.jpg');}
        img    { border: solid black 1px; margin:10px; }
        object { border: solid black 1px; margin:10px; }
        h1   { text-shadow: 2px 2px 2px #000;
               font-size:105px; color:#fff; border-bottom: solid black 1px;
               font-size: 300%%; font-family: 'Yanone Kaffeesatz'}
        a, a:active, a:visited {
               color:#FADA00; text-decoration:none; }
        a:hover{ color:#FFFF00; text-decoration:none;
                 text-shadow: 0px 0px 5px #fff; }
    </style>
    <title>%(path)s</title>
    </head>
    <body>
    <a style="" href="http://packages.python.org/rootplot/"><img style="position: absolute; top:10 px; right: 10px; border: 0px" src="http://packages.python.org/rootplot/_static/rootplot-logo.png"></a>
    <h1>Navigation</h1>
      %(back_nav)s
      <ul>
          %(forward_nav)s
      </ul>
    <h1>Images</h1>
    %(plots)s
    <p style='font-size: x-small; text-align: center;'>
      <a href='http://www.greepit.com/resume-template/resume.htm'>
        Based on a template by Sarfraz Shoukat</a></p>
    </body>
    </html>
'''