#------------------------------------------------------------------------------#
#                                                                              #
#                        Hands-On Data Visualization in R:                     # 
#                   How to create engaging charts with ggplot2                 #
#                                                                              #
#                     Dr. Cédric Scherer | January 14, 2022                    #
#                                                                              #
#------------------------------------------------------------------------------#


## PACKAGES ####################################################################

## install.packages("ggplot2")
## library(ggplot2)

## install.packages("tidyverse")
library(tidyverse)

## install.packages("magick")
## install.packages("ggtext")
## install.packages("ggforce")
## install.packages("ggrepel")
## install.packages("patchwork")



## DATA ########################################################################

chic <- read_csv(
  "https://raw.githubusercontent.com/z3tt/ggplot-courses/master/data/chicago-nmmaps-custom.csv",
  col_types = cols(season = col_factor(), year = col_factor())
)

glimpse(chic)



## GGPLOT(DATA) ################################################################

?ggplot

ggplot(data = chic)

ggplot(data = chic)



## AESTHETICS ##################################################################

ggplot(
  data = chic,
  mapping = aes(x = date, y = temp)
)

ggplot(
  data = chic,
  mapping = aes(x = date, y = temp)
)

ggplot(chic, aes(date, temp))

ggplot(chic, aes(x = date, y = temp))



## GEOMETRICAL LAYERS ##########################################################

ggplot(chic, aes(x = date, y = temp)) +
  geom_point()

ggplot(chic, aes(x = date, y = temp)) +
  geom_area()



## THEMES ######################################################################

ggplot(chic, aes(x = date, y = temp)) +
  geom_point() +
  theme_classic()

ggplot(chic, aes(x = date, y = temp)) +
  geom_point() +
  theme_minimal()

ggplot(chic, aes(x = date, y = temp)) +
  geom_point() +
  theme_bw()

ggplot(chic, aes(x = date, y = temp)) +
  geom_point() +
  theme_dark()

ggplot(chic, aes(x = date, y = temp)) +
  geom_point() +
  theme_light()

theme_set(theme_light())

ggplot(chic, aes(x = date, y = temp)) +
  geom_point()

## note that the font needs to be installed locally and the agg device needs to 
## be activated in the Rstudio settings to see the changes in the plots pane:
## Tools -> Global Options -> General -> Graphics -> Backend
ggplot(chic, aes(x = date, y = temp)) +
  geom_point() +
  theme_light(
    base_size = 35,
    base_family = "Consolas"
  )


## GEOMETRICAL LAYERS (CONTINUED) ##############################################


## EXERCISE

# Turn our scatter plot into a line chart and into a bar chart!
  
# What’s the difference between `geom_path()` and `geom_line()`?
  
# Create a box plot of temperature per date.

# What is the problem? How could you find out why this is happening?


ggplot(chic, aes(x = date, y = temp)) +
  geom_point() +
  geom_line() +
  geom_rug(sides = "r")



## LABELS ######################################################################

ggplot(chic, aes(x = date, y = temp)) +
  geom_point() +
  xlab("Day of the Year") +
  ylab("Temperature (°F)")

ggplot(chic, aes(x = date, y = temp)) +
  geom_point() +
  xlab("Day of the Year") +
  ylab("Temperature (°F)") +
  ggtitle("Seasonal Change of Temperatures in Chicago")

ggplot(chic, aes(x = date, y = temp)) +
  geom_point() +
  labs(
    x = "Day of the Year",
    y = "Temperature (°F)",
    title = "Seasonal Change of Temperatures in Chicago",
    subtitle = "Daily temperatures (°F) in the city of Chicago, IL,\nmeasured between 1997 and 2001",
    caption = "Data: NMMAPS",
    tag = "Fig. 1"
  )



## ASSIGNING TO AN OBJECT ######################################################

g <- 
  ggplot(chic, aes(x = date, y = temp)) +
  geom_point()

g

g +
  geom_line() +
  geom_rug(sides = "r")

g +
  geom_smooth()



## SAVING PLOTS ################################################################

ggsave(filename = "my_ggplot.png",
       width = 10, height = 7,
       dpi = 700)

ggsave(filename = "my_ggplot.pdf",
       width = 10, height = 7,
       device = cairo_pdf)



## STATISTICAL LAYERS ##########################################################

ggplot(chic, aes(x = date, y = temp)) +
  stat_smooth(geom = "smooth")

ggplot(chic, aes(x = date, y = temp)) +
  geom_smooth(stat = "smooth")

ggplot(chic, aes(x = temp)) +
  stat_count(geom = "bar")

ggplot(chic, aes(x = temp)) +
  geom_bar(stat = "count")

ggplot(chic, aes(x = date, y = temp)) +
  stat_identity(geom = "point")

ggplot(chic, aes(x = date, y = temp)) +
  geom_point(stat = "identity")

ggplot(chic, aes(x = season, y = temp)) +
  stat_summary()

ggplot(chic, aes(x = season, y = temp)) +
  stat_summary(
    fun.data = mean_se, ## the default
    geom = "pointrange" ## the default
  )

ggplot(chic, aes(x = season, y = temp)) +
  stat_summary(
    fun.data = mean_se, ## the default
    geom = "errorbar", 
    width = .3
  )

ggplot(chic, aes(x = season, y = temp)) +
  stat_summary(
    fun = mean,
    fun.max = function(y) mean(y) + sd(y),
    fun.min = function(y) mean(y) - sd(y)
  )

ggplot(chic, aes(x = season, y = temp)) +
  stat_summary(
    fun = mean,
    size = 1,
    color = "#28a87d"
  )

ggplot(chic, aes(x = season, y = temp)) +
  geom_boxplot() +
  stat_summary(
    fun = mean,
    size = 1,
    color = "#28a87d"
  )



## AESTHETICS (AGAIN) ##########################################################

ggplot(chic, aes(x = date, y = temp)) +
  geom_point(
    size = 4,
    alpha = .3
  )

ggplot(chic, aes(x = date, y = temp)) +
  geom_point(
    aes(color = season, shape = year),
    size = 4,
    alpha = .3
  )

ggplot(chic, aes(x = date, y = temp, color = season, shape = year)) +
  geom_point(
    size = 4,
    alpha = .3
  )

ggplot(chic, aes(x = date, y = temp)) +
  geom_line(aes(color = season))

ggplot(chic, aes(x = date, y = temp)) +
  geom_line(aes(color = season, group = year))

ggplot(chic, aes(x = date, y = temp)) +
  geom_line(aes(color = season, group = year)) +
  stat_smooth(color = "black")

ggplot(chic, aes(x = date, y = temp, group = year)) +
  geom_line(aes(color = season)) +
  stat_smooth(color = "black")

ggplot(chic, aes(x = date, y = temp, color = season)) +
  geom_point() +
  scale_x_date() + 
  scale_y_continuous() + 
  scale_color_discrete()

ggplot(chic, aes(x = date, y = temp, color = season)) +
  geom_point() +
  scale_x_date(
    expand = c(0, 0), ## general
    date_breaks = "6 months", ## date only
    date_labels = "%m/%y", ## date only
    name = NULL ## general 
  ) +
  scale_y_continuous() +
  scale_color_discrete()



ggplot(chic, aes(x = date, y = temp, color = season)) +
  geom_point() +
  scale_x_date(
    expand = c(0, 0), ## general
    date_breaks = "6 months", ## date only
    date_labels = "%m/%y", ## date only
    name = NULL ## general
  ) +
  scale_y_continuous(
    breaks = seq(-10, 90, by = 10),  ## general 
    labels = function(y) paste0(y, "°F"),  ## general 
    name = "Temperature" ## general 
  ) +
  scale_color_discrete()

ggplot(chic, aes(x = date, y = temp, color = season)) +
  geom_point() +
  scale_x_date(
    expand = c(0, 0), ## general
    date_breaks = "6 months", ## date only
    date_labels = "%m/%y", ## date only
    name = NULL ## general
  ) +
  scale_y_continuous(
    breaks = seq(-10, 90, by = 10),  ## general
    labels = function(y) paste0(y, "°F"), ## general
    name = "Temperature" ## general
  ) +
  scale_color_discrete(
    type = c("#91A3E1", "#83B692", "#F9ADA0", "#CD6FD5"), ## color only
    name = "Season:" ## general 
  )



### COLOR-SCALES

### qualitative (`discrete`)

ggplot(chic, aes(x = date, y = temp, color = season)) + 
  geom_point() +
  scale_color_discrete(
    type = c("#91A3E1", "#83B692", "#F9ADA0", "#CD6FD5")
  )

ggplot(chic, aes(x = date, y = temp, color = season)) + 
  geom_point() +
  scale_color_manual(
    values = c("#91A3E1", "#83B692", "#F9ADA0", "#CD6FD5")
  )

ggplot(chic, aes(x = date, y = temp, color = season)) +
  geom_point() +
  scale_color_viridis_d(
    option = "mako"
  )

ggplot(chic, aes(x = date, y = temp, color = season)) + 
  geom_point() +
  scale_color_viridis_d(
    option = "turbo"
  )

ggplot(chic, aes(x = date, y = temp, color = season)) +
  geom_point() +
  scale_color_brewer(
    palette = "Set1"
  )

ggplot(chic, aes(x = date, y = temp, color = season)) +
  geom_point() +
  scale_color_brewer(
    palette = "Set2"
  )

### quantitative (`continuous`)

ggplot(chic, aes(x = date, y = temp, color = o3)) + 
  geom_point() +
  scale_color_continuous(
    type = "viridis"
  )

ggplot(chic, aes(x = date, y = temp, color = o3)) +  
  geom_point() +
  scale_color_gradient(
    low = "#e8d8c3", high = "#1e4511"
  )

ggplot(chic, aes(x = date, y = temp, color = o3)) +
  geom_point() +
  scale_color_viridis_c(
    option = "mako", direction = -1
  )

ggplot(chic, aes(x = date, y = temp, color = o3)) + 
  geom_point() +
  scale_color_viridis_c( 
    option = "inferno", end = .9
  ) 

ggplot(chic, aes(x = date, y = temp, color = o3)) +
  geom_point() +
  scale_color_distiller(
    palette = "PuRd"
  )

ggplot(chic, aes(x = date, y = temp, color = o3)) +
  geom_point() +
  scale_color_distiller(
    palette = "Spectral"
  )


### OTHER SCALES

ggplot(chic, aes(x = date, y = temp, shape = year)) + 
  geom_point(fill = "red") +
  scale_shape_manual(
    values = 21:24
  )

ggplot(chic, aes(x = date, y = temp, linetype = year)) + 
  geom_line() +
  scale_linetype_manual(
    values = 3:6
  )

ggplot(chic, aes(x = date, y = temp, size = o3)) + 
  geom_point(alpha = .5) +
  scale_size(
    range = c(.5, 3.5)
  )

ggplot(chic, aes(x = date, y = temp, alpha = o3)) + 
  geom_point() +
  scale_alpha(
    range = c(.2, .8)
  )


## EXERCISE

# A training data set focusing on cars comes with `ggplot2` and is called `mpg`.
# Create the following visualization:



## COORDINATE SYSTEMS ##########################################################

ggplot(chic, aes(x = date, y = temp)) +
  geom_line() +
  coord_cartesian(ylim = c(50, 70))

ggplot(chic, aes(x = date, y = temp)) +
  geom_line() +
  scale_y_continuous(limits = c(50, 70))

ggplot(chic, aes(x = season, y = temp)) +
  geom_boxplot()  +
  coord_cartesian(ylim = c(50, 70))

ggplot(chic, aes(x = season, y = temp)) +
  geom_boxplot()  +
  scale_y_continuous(limits = c(50, 70))

ggplot(chic, aes(x = o3, y = dewpoint)) +
  geom_point() +
  coord_cartesian()

ggplot(chic, aes(x = o3, y = dewpoint)) +
  geom_point() +
  coord_fixed()

ggplot(chic, aes(x = season, y = temp)) +
  geom_boxplot() +
  coord_cartesian()

ggplot(chic, aes(x = season, y = temp)) +
  geom_boxplot() +
  coord_flip()

ggplot(chic, aes(x = temp, y = season)) +
  geom_boxplot() +
  coord_cartesian()

ggplot(chic, aes(season, temp, fill = season)) +
  stat_summary(fun = mean, geom = "col") +
  coord_cartesian()

ggplot(chic, aes(season, temp, fill = season)) +
  stat_summary(fun = mean, geom = "col") +
  coord_polar()

ggplot(chic, aes(1, fill = season)) +
  stat_count() + 
  coord_cartesian()

ggplot(chic, aes(1, fill = season)) +
  stat_count() + 
  coord_polar(theta = "y")



### FACETTING ##################################################################

g <- 
  ggplot(chic, aes(temp, o3)) +
  geom_point(
    aes(color = year),
    alpha = .5
  )

g

g +
  facet_wrap(vars(year))

g +
  facet_wrap(~ year)

g +
  facet_wrap(
    vars(year),
    scales = "free"
  )

g +
  facet_wrap(
    vars(year),
    scales = "free_x"
  )

g +
  facet_wrap(
    vars(year),
    scales = "free_x",
    nrow = 4
  )

g +
  facet_grid(
    vars(season), 
    vars(year)
  )

g +
  facet_grid(
    season ~ year
  )

ggplot(chic, aes(temp, o3)) +
  geom_point(
    aes(color = year),
    alpha = .5
  ) +
  facet_grid(
    year ~ season
  )

g +
  facet_grid(
    year ~ season,
    scales = "free"
  )

ggplot(chic, aes(temp, o3)) +
  geom_point(
    aes(color = year),
    alpha = .5
  ) +
  facet_grid(
    year ~ season,
    scales = "free",
    space = "free"
  )



## THEME ELEMENTS ##############################################################

g +
  theme(
    plot.background = element_rect(
      color = "orange",
      fill = "black",
      size = 2,
      linetype = "dotted"
    ),
    axis.title = element_text(
      family = "Bangers",
      color = "blue",
      angle = 15,
      size = 40
    )
  )

g +
  theme(
    axis.ticks = element_blank(),
    legend.title = element_blank()
  )

g +
  ggtitle("My Title") +
  theme(
    plot.title.position = "panel",
    legend.position = "top"
  )

g +
  ggtitle("My Title") +
  theme(
    plot.title.position = "plot",
    legend.position = c(.1, .85)
  )



## ANNOTATION ##################################################################

ggplot(chic, aes(x = temp, y = o3)) +
  annotate(
    geom = "rect",
    xmin = 70,
    xmax = Inf,
    ymin = -Inf,
    ymax = 25,
    fill = "red",
    color = "firebrick"
  ) +
  geom_point(alpha = .4)

ggplot(chic, aes(x = dewpoint, y = temp)) +
  geom_point(size = 2) +
  annotate(
    geom = "text",
    x = 65, y = 10,
    label = "Some\nadditional\ntext",
    size = 6,
    lineheight = .9
  ) +
  annotate(
    geom = "curve",
    x = 65, y = 20,
    xend = 51, yend = 49,
    arrow = arrow(length = unit(.5, "cm"))
  )

library(magick)

url <- "https://seeklogo.com/images/C/Chicago-logo-949A99BB67-seeklogo.com.png"
img <- image_read(url)

ggplot(chic, aes(x = date, y = temp)) +
  annotation_custom(
    grid::rasterGrob(
      image = img,
      width = .7
    )
  ) +
  geom_point(color = "red")


## EXTENSION PACKAGES ##########################################################

### {ggtext}

library(ggtext)

g +
  labs(
    title = "*My* **Title**",
    x = "<i style='color:red;'>Temperature</i>",
    y = "<b style='font-family:lora;'>Ozone</b> Levels"
  ) +
  theme(
    plot.title = element_markdown(),
    axis.title.x = element_markdown(),
    axis.title.y = element_markdown()
  )


### {ggforce}

library(ggforce)

ggplot(chic, aes(x = temp, y = yday,
                 color = season)) +
  geom_point() +
  geom_mark_ellipse(
    aes(label = season)
  )

ggplot(chic, aes(x = temp, y = yday,
                 color = season)) +
  geom_point() +
  geom_mark_hull(
    aes(label = season,
        filter = season == "Summer")
  )


### {ggrepel}

ggplot(filter(chic, temp > 80, o3 > 40),
       aes(x = date, y = temp)) +
  geom_point() +
  geom_text(aes(label = season))

library(ggrepel)

ggplot(filter(chic, temp > 80, o3 > 40),
       aes(x = date, y = temp)) +
    geom_point() +
    geom_text_repel(aes(label = season))


### {patchwork}

(time <- 
  ggplot(chic, aes(x = date, y = temp)) +
  geom_point())

(summary <- 
  ggplot(chic, aes(x = season, y = temp)) +
  geom_boxplot())

library(patchwork)

time + summary

time / summary

time + summary +
  plot_layout(
    widths = c(1, .5)
  ) +
  plot_annotation(
    title = "My very long title spanning both plots",
    tag_levels = "A",
    tag_suffix = ")"
  )

## END #########################################################################
