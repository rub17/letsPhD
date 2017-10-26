double getCosTheta (TLorentzVector restParticle, TLorentzVector particleOne,TLorentzVector particleTwo) {//Due to simpler definitions, with theta angle we can simply give the two particles and boost them into the wanted frame. That part is all done by the routine. The resulting angle between them will be the theta angle (z-axis is the W boson).
    
    TLorentzVector restFrame = restParticle;
    TLorentzVector particle1 = particleOne;
    TLorentzVector particle2 = particleTwo;
    
    particle1.Boost(-restFrame.BoostVector());
    particle2.Boost(-restFrame.BoostVector());
    
    double Angle = cos(particle1.Angle(particle2.Vect()));
    return Angle;
};

double getPhi (TLorentzVector restParticle, TLorentzVector particle,TVector3 x_axis, TVector3 y_axis,TVector3 z_axis) { //Due to the relatively complex setup, we will need x-axis in the right frame to be provided for the routine; in other words, it does not boost the last argument input. Double check if the x-axis is boosted in the frame that's described in the first input.
    
    TLorentzVector restFrame = restParticle;
    TLorentzVector p = particle;
    TVector3 x = x_axis;
    TVector3 y = y_axis;
    TVector3 z = z_axis;
    TVector3 pVect;
    
    p.Boost(-restFrame.BoostVector());
    pVect = p.Vect();
    pVect = pVect - pVect.Dot(z)*z;
    double pVect_X = pVect.Dot(x);
    double pVect_Y = pVect.Dot(y);
    double Angle = atan2(pVect_Y,pVect_X);
    
    return Angle;
};

