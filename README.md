# galactic-war
A Galactic Warfare Simulation 

This program receives as input a series of “battalion deployments,” or placements of troops on a certain planet. 

A battalion deployment consists of the following information:
  ● Timestamp — the time that this deployment order is issued.
  ● General ID — the general who is issuing the deployment order.
  ● Planet ID — the planet which the troops are being deployed to.
  ● Jedi or Sith — Whether the General issuing the deployment is a Jedi or Sith.
  ● Force-sensitivity — the average Force-sensitivity of the troops being deployed.
  ● Quantity — the number of troops being deployed.
  
As each battalion deployment is read from input, the program sees if the new battalion can be matched with a battalion previously deployed on the planet. If a match occurs, then the two battalions engage in warfare. A new battalion can be matched with a previously deployed battalion if:
  ● Both deployments are for the same planet.
    ○ General ID does not matter; generals are allowed to switch sides of the Force: i.e. order conflicting battalion deployments.
  ● The deployments were issued on different sides of the Force. That is, one deployment was a Jedi deployment and the other was a Sith deployment.
  ● The Jedi Force-sensitivity is less than or equal to the Sith Force-sensitivity.
    ○ The Sith always instigate fights, and they only fight battalions with Force-sensitivity which they are confident that they can overcome.
    ○ This does not mean that a battle will not occur if a Jedi battalion is being deployed. When a Jedi battalion is deployed, they may be ambushed by a previously
      deployed Sith battalion.
 
If the new battalion is a Sith battalion, it will always choose to attack the least Force-sensitive Jedi battalion on the planet, given that there is a Jedi battalion with lesser Force-sensitivity. If the new battalion is a Jedi battalion, it will always be ambushed by the most Force-sensitive Sith battalion on the planet, given that there is a Sith battalion with greater Force-sensitivity. In the event of a tie in Force-sensitivity, choose the battalion that was deployed first (came first in the input file).

When a battle occurs, the battalions trade troops one-for-one, regardless of their Force-sensitivity. That is to say that an equal number of troops from both battalions are eliminated, equal to the number of troops in the smaller battalion. If one of the battalions survives, it remains on the planet for future possible fights. For example, if a Sith battalion with 20 troops fights a Jedi battalion of 30 troops, the Sith battalion is eradicated and the Jedi battalion remains with 10 troops. In the event that the newly deployed battalion survives, it is possible that a new fight will break out. If the new battalion is Sith, they will then look to attack another Jedi battalion. If the new battalion is Jedi, they may be attacked again after defeating the first Sith battalion. This happens until no more fights break out: that is, there are no pairs of Jedi and Sith battalions remaining on the planet such that the Sith Force-sensitivity is greater than or equal to the Jedi Force-sensitivity.
